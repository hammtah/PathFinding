#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>
#include <climits>

#include "DijkstraPq.h"
using namespace std;
#include "DijkstraPq.h"
#include "Grid.h"
void printPath(std::vector<int> path) {
    for (int i = 0; i < path.size() - 1; i++) {
        cout << path[i] << " -> ";
    }
    cout << path.back() << endl;
}

int main1() {
    cout << "Hello World!" << endl;
    std::vector<std::vector<int>> testMatrix = {
        //       Node: 0    1    2    3    4
        /*Node 0*/ {  1,  1,   1, INF, INF},
        /*Node 1*/ {INF,   1, INF,   1, INF},
        /*Node 2*/ {INF,   1,   1,   1,   1},
        /*Node 3*/ {INF, INF, INF,   1, INF},
        /*Node 4*/ {INF, INF, INF,   1,   1}
    };

    // Dijkstra d;
    // printPath(d.shortestPath(testMatrix, 0, 4));
    DijkstraPQ dijkstra_pq;
    // printPath(dijkstra_pq.shortestPath(testMatrix, 0, 4));


        int width= 10;
        int height= 10;
        pii start= {0,0};
        pii end= {3,2};
        vector<pii> obstacles= {{1,0}, {1,1}, {1,2}};


    auto a = gridToList(start, end, obstacles, width, height);
    int startId = generateCellId(start.first, start.second, width);
    int endId = generateCellId(end.first, end.second, width);
    auto res = dijkstra_pq.dijkstraPQ(a.size(), a, startId, endId);
   printPath(dijkstra_pq.recover(res.prev,endId));
    return 0;

    /*
    {
        "width": 10,
        "height": 10,
        "end":{"x": 3,"y":2},
        "start":{"x":0,"y":0},
        "obstacles":[{"x":1,"y":0}, {"x":1,"y":1}, {"x":1,"y":2}]
    }
     */
}

#include "crow.h"
#include "crow/middlewares/cors.h"

bool valid(crow::json::rvalue json) {
    if (json["width"] && json["height"]  && json["start"] && json["end"] && json["obstacles"]) {
        if (json["start"]["x"] && json["start"]["y"] && json["end"]["x"] && json["end"]["y"] && json["width"].i() > 0 && json["height"].i() > 0)
            return true;
    }
    return false;
}
int main() {
    DijkstraPQ dijkstra_pq;
    // Initialize the app with CORS middleware
    crow::App<crow::CORSHandler> app;
    // Configure CORS
    auto& cors = app.get_middleware<crow::CORSHandler>();

    cors
      .global()                         // Apply to all routes
      .origin("http://localhost:5173")  // Allow React app
      .methods("POST"_method, "GET"_method, "OPTIONS"_method)
      .headers("Content-Type", "Authorization");

    CROW_ROUTE(app, "/api/path").methods("POST"_method)([&dijkstra_pq](const crow::request& req){
        auto data = crow::json::load(req.body);

        //Validation
        if (!data) return crow::response(400, "Invalid JSON");
        if (!valid(data)) return crow::response(400, "Invalid JSON");
        //Extract input
        int width = data["width"].i();
        int height = data["height"].i();
        pii start = {data["start"]["x"].i(), data["start"]["y"].i()};
        pii end = {data["end"]["x"].i(), data["end"]["y"].i()};
        std::vector<pii> obstacles;
        for (auto& obs : data["obstacles"]) {
            obstacles.push_back({obs["x"].i(), obs["y"].i()});
        }

        //Call the algorithm
        auto list = gridToList(start, end, obstacles, width, height);
        int startId = generateCellId(start.first, start.second, width);
        int endId = generateCellId(end.first, end.second, width);
        auto res = dijkstra_pq.dijkstraPQ(list.size(), list, startId, endId);
        auto idsPath = dijkstra_pq.recover(res.prev,generateCellId(end.first, end.second, width));
        auto idsVisited = res.visited;
        //Prepare Coordinate Path for Frontend
        std::vector<crow::json::wvalue> path;
        for (auto id : idsPath) {
            auto coord = getCoord(id, width);
            crow::json::wvalue c;
            c["x"] = coord.x;
            c["y"] = coord.y;
            path.push_back(std::move(c));
        }
        //Prepare Coordinate visited for Frontend
        std::vector<crow::json::wvalue> visited;
        for (auto id : idsVisited) {
            auto coord = getCoord(id, width);
            crow::json::wvalue c;
            c["x"] = coord.x;
            c["y"] = coord.y;
            visited.push_back(std::move(c));
        }
        // Create the JSON object
        crow::json::wvalue response;
        response["path"] = std::move(path);
        response["status"] = "success";
        response["visited"] = std::move(visited);
//         response["distance"] = -1;
// if (endId < res.dist.size()) response["distance"] = res.dist[endId] == 1e9 ? -1 : res.dist[endId];

        response["distance"] = res.dist[endId] == 1e9 ? -1 : res.dist[endId];
        auto crow_res = crow::response(response);
        crow_res.set_header("Access-Control-Allow-Origin", "*");
        return crow_res;
    });

    app.port(18080).multithreaded().run();

}