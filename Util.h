//
// Created by taha on 2/4/26.
//

#ifndef PATH_FINDING_UTIL_H
#define PATH_FINDING_UTIL_H
#include <crow/json.h>
#include <vector>
#include <ranges>

class Util {
public:
    static bool valid(crow::json::rvalue json) {
        if (json["width"] && json["height"]  && json["start"] && json["end"] && json["obstacles"] && json["algo"]) {
            if (json["start"]["x"] && json["start"]["y"] && json["end"]["x"] && json["end"]["y"] && json["width"].i() > 0 && json["height"].i() > 0 )
                return true;
        }
        return false;
    }
    static int recalcUnweightedDistance(std::vector<int> path, std::vector<pii> swamps, int swampWeight, int width) {
        int numOfSwampsInPath = 0;
        for (auto swamp : swamps) {
            if (find(path.begin(), path.end(), generateCellId(swamp.first, swamp.second, width)) != path.end()) {
                numOfSwampsInPath++;
            };
        }
        return ((path.size() - numOfSwampsInPath) + numOfSwampsInPath*swampWeight);
    }

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

};
#endif //PATH_FINDING_UTIL_H