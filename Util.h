//
// Created by taha on 2/4/26.
//

#ifndef PATH_FINDING_UTIL_H
#define PATH_FINDING_UTIL_H
#include <crow/json.h>
#include <vector>
#include <ranges>
#include <iostream>
#include <chrono>


/**
 * @brief Utility helpers for request validation, distance post-processing, and JSON conversion.
 *
 * This class provides:
 *  - Basic validation for incoming JSON payloads,
 *  - Recalculation of path distance for unweighted algorithms when weighted cells (e.g., swamps) exist,
 *  - Conversion utilities from node ids to JSON coordinate objects for responses.
 */
class Util {
public:
    /**
     * @brief Validate the presence and basic correctness of required JSON fields.
     *
     * Checks for width, height, start/end coordinates, obstacles array, and algorithm key.
     * Ensures width and height are positive and that start/end have x and y members.
     *
     * @param json Parsed crow::json::rvalue representing the request body.
     * @return true if the minimal required fields exist and are of expected types; false otherwise.
     * @complexity O(1).
     * @note This is a shallow validation; deeper semantic checks (e.g., bounds) are performed elsewhere.
     */
    static bool valid(crow::json::rvalue json) {
        if (json["width"] && json["height"]  && json["start"] && json["end"] && json["obstacles"] && json["algo"]) {
            if (json["start"]["x"] && json["start"]["y"] && json["end"]["x"] && json["end"]["y"] && json["width"].i() > 0 && json["height"].i() > 0 )
                return true;
        }
        return false;
    }

    /**
     * @brief Recompute effective path distance accounting for weighted cells along the path.
     *
     * Treats each swamp cell along the given path as having cost @p swampWeight instead of 1.
     * Non-swamp cells continue to contribute cost 1. The final distance is:
     *   (|path| - numSwamps) + numSwamps * swampWeight
     *
     * @param path         Sequence of visited node ids forming a path.
     * @param swamps       List of swamp coordinates (x, y).
     * @param swampWeight  Cost of traversing a swamp cell.
     * @param width        Grid width used to map (x, y) to linear ids.
     * @return The adjusted path distance considering swamps.
     * @complexity O(|swamps| * |path|) due to linear search for membership per swamp.
     * @note Assumes node ids in @p path are consistent with generateCellId(x, y, width).
     */
    static int recalcUnweightedDistance(std::vector<int> path, std::vector<pii> swamps, int swampWeight, int width) {
        int numOfSwampsInPath = 0;
        for (auto swamp : swamps) {
            if (find(path.begin(), path.end(), generateCellId(swamp.first, swamp.second, width)) != path.end()) {
                numOfSwampsInPath++;
            };
        }
        return ((path.size() - numOfSwampsInPath) + numOfSwampsInPath*swampWeight);
    }

    /**
     * @brief Convert a vector of node ids into a vector of JSON coordinate objects.
     *
     * Each node id is mapped to (x, y) using getCoord(id, width) and serialized as:
     *   {"x": x, "y": y}
     *
     * @param v      Vector of node ids.
     * @param width  Grid width used by getCoord.
     * @return A vector of crow::json::wvalue objects representing coordinates.
     * @complexity O(n), where n = v.size().
     */
    static std::vector<crow::json::wvalue> vectorToWJsonCoords(std::vector<int> v, int width) {
        std::vector<crow::json::wvalue> wv;
        for (auto id : v) {
            auto coord = getCoord(id, width);
            crow::json::wvalue c;
            c["x"] = coord.x;
            c["y"] = coord.y;
            wv.push_back(std::move(c));
        }
        return wv;
    }

    /**
     * @brief Provides functionality for measuring time intervals.
     *
     * This class is designed to compute elapsed time between the start and stop points.
     * It can be used to benchmark performance or track time durations for operations.
     */
    struct Timer {
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        float *_out;
        Timer(float* duration) {
            start = std::chrono::high_resolution_clock::now();
            _out = duration;
        }

        ~Timer() {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> duration = end - start;
            if (_out)
                *_out = duration.count() * 1000.0f ;
        }
    };

};
#endif //PATH_FINDING_UTIL_H