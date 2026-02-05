//
// Created by taha on 2/4/26.
//

#ifndef PATH_FINDING_SERVER_H
#define PATH_FINDING_SERVER_H
#include "crow.h"
#include "crow/middlewares/cors.h"

/**
 * @brief Incoming path-finding request payload decoded from JSON.
 *
 * Represents grid dimensions, start/end coordinates, algorithm selection,
 * and environment features such as obstacles and swamps.
 */
struct PathRequest {
    int width;                 ///< Grid width (number of columns).
    int height;                ///< Grid height (number of rows).
    pii start;                 ///< Start cell as (x, y).
    pii end;                   ///< End cell as (x, y).
    std::string algo;          ///< Algorithm key (e.g., "dij", "astar", "bfs").
    std::vector<pii> obstacles;///< List of obstacle cells as (x, y).
    std::vector<pii> swamps;   ///< List of swamp cells as (x, y) with higher traversal cost.
};

/**
 * @brief Preprocessed inputs for path-finding algorithms.
 *
 * Contains an adjacency list and linearized ids of the start and end cells.
 */
struct AlgosFeed {
    std::vector<std::vector<Edge>> list; ///< Adjacency list for the grid.
    int startId;                         ///< Linear id of the start cell.
    int endId;                           ///< Linear id of the end cell.
};

/**
 * @brief Build the adjacency list and compute linear ids for start and end cells.
 *
 * @param requestData Parsed request data containing grid dimensions, start/end, obstacles, and swamps.
 * @return AlgosFeed with adjacency list and start/end node ids.
 * @note The adjacency list encodes 4-directional movement, obstacles as impassable, and swamps as higher-cost cells.
 */
AlgosFeed generateAlgosFeed(PathRequest requestData) {
    auto list = gridToList(requestData.start, requestData.end, requestData.obstacles, requestData.width, requestData.height, requestData.swamps);
    int startId = generateCellId(requestData.start.first, requestData.start.second, requestData.width);
    int endId = generateCellId(requestData.end.first, requestData.end.second, requestData.width);
    return {list, startId, endId};
}

/**
 * @brief Extract and validate request fields from a JSON payload.
 *
 * Expected schema:
 * {
 *   "width": int,
 *   "height": int,
 *   "start": {"x": int, "y": int},
 *   "end":   {"x": int, "y": int},
 *   "algo":  string,
 *   "obstacles": [{"x": int, "y": int}, ...],
 *   "swamps":    [{"x": int, "y": int}, ...]
 * }
 *
 * @param data Parsed crow JSON rvalue.
 * @return PathRequest with all fields populated.
 * @throws (indirectly) if required fields are missing or have invalid types.
 */
PathRequest getRequestData(crow::json::rvalue data) {
    //Extract input
    int width = data["width"].i();
    int height = data["height"].i();
    pii start = {data["start"]["x"].i(), data["start"]["y"].i()};
    pii end = {data["end"]["x"].i(), data["end"]["y"].i()};
    std::string algo = data["algo"].s();
    std::vector<pii> obstacles;
    for (auto& obs : data["obstacles"]) {
        obstacles.push_back({obs["x"].i(), obs["y"].i()});
    }
    std::vector<pii> swamps;
    for (auto& obs : data["swamps"]) {
        swamps.push_back({obs["x"].i(), obs["y"].i()});
    }
    return {width, height, start, end, algo, obstacles, swamps};
}

/**
 * @brief HTTP server exposing path-finding APIs.
 *
 * Configures CORS, defines REST endpoints, and runs the event loop.
 */
class Server {
    crow::App<crow::CORSHandler> app; ///< Crow application with CORS middleware.
    PathFinder pathFinder;            ///< Algorithm dispatcher/runner.

    /**
     * @brief Configure global CORS policy for the app.
     *
     * Sets allowed origin, methods, and headers.
     * @sideeffects Mutates the middleware configuration on the internal app instance.
     */
    void bootstrap() {
        // Configure CORS
        auto& cors = app.get_middleware<crow::CORSHandler>();
        cors
          .global()                         // Apply to all routes
          .origin("http://localhost:5173")  // Allow React app
          .methods("POST"_method, "GET"_method, "OPTIONS"_method)
          .headers("Content-Type", "Authorization");
    }
    /*
    crow::response error(std::string& msg, int errorCode) {
        return crow::response(errorCode, msg);
    }
    int getIntField(std::string& field) {
        return getField(field).i();
    }
    std::string getStrField(std::string& field) {
        return getField(field).s();
    }
    crow::json::rvalue getField(std::string& field) {
        if (!data.has(field)) throw std::runtime_error("Invalid field");
        return data[field];
    }
    void setResponseAttribute(std::string& attribute, crow::json::rvalue value) {
        response[attribute] = value;
    }
     */

public:
    /**
     * @brief Construct the server and initialize configuration.
     *
     * Calls bootstrap() to configure CORS and other middleware.
     */
    Server(){
        bootstrap();
    }

    /**
     * @brief Define routes and start the HTTP server.
     *
     * Exposes POST /api/path which:
     *  - Validates the request body,
     *  - Parses request data,
     *  - Generates algorithm inputs,
     *  - Runs the selected algorithm ("dij", "astar", or "bfs"),
     *  - Returns a JSON response containing path, visited nodes, and distance.
     *
     * The server listens on port 18080 and runs multi-threaded.
     */
    void serve(){
        CROW_ROUTE(app, "/api/path").methods("POST"_method)([this](const crow::request& req){
            auto data = crow::json::load(req.body);

            //Validation
            if (!data) return crow::response(400, "Invalid JSON");
            if (!Util::valid(data)) return crow::response(400, "Invalid JSON");

            //Extract request Data
            auto requestData = getRequestData(data);

            //Prepare data for algorithm
            auto algoFeed = generateAlgosFeed(requestData);

            //Call the algorithm
            PathResult res;
            if (requestData.algo == "dij") {
                res = pathFinder.findPathDijkstra(algoFeed.list.size(), algoFeed.list, algoFeed.startId, algoFeed.endId);
            }else if (requestData.algo == "astar") {
                res = pathFinder.findPathAstar(algoFeed.list.size(), algoFeed.list, algoFeed.startId, algoFeed.endId, requestData.width);
            }
            else if (requestData.algo == "bfs") {
                res = pathFinder.findPathBFS(algoFeed.list.size(), algoFeed.list, algoFeed.startId, algoFeed.endId);
                //Recalculate distance so that it calculate weighted nodes as well:
                //If the path contains a swamp then change the weight of this cell to the swamp's weight instead of 1
                //This is helpful for comparison between algos
                if (res.dist[algoFeed.endId] != 1e9 ) res.dist[algoFeed.endId] = Util::recalcUnweightedDistance(res.path, requestData.swamps, 5, requestData.width);
            }

            //Prepare Coordinate Path for Frontend
            std::vector<crow::json::wvalue> path = std::move(Util::vectorToWJsonCoords(res.path, requestData.width));

            //Prepare Coordinate visited for Frontend
            std::vector<crow::json::wvalue> visited = std::move(Util::vectorToWJsonCoords(res.visited, requestData.width));

            // Create the JSON object
            crow::json::wvalue response;
            response["path"] = std::move(path);
            response["status"] = "success";
            response["visited"] = std::move(visited);
            response["distance"] = res.dist[algoFeed.endId] == 1e9 ? -1 : res.dist[algoFeed.endId];

            auto crow_res = crow::response(response);
            crow_res.set_header("Access-Control-Allow-Origin", "*");
            return crow_res;
        });

        app.port(18080).multithreaded().run();
    }
};
#endif //PATH_FINDING_SERVER_H