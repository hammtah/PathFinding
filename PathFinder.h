//
// Created by taha on 2/2/26.
//

#ifndef PATH_FINDING_PATHFINDER_H
#define PATH_FINDING_PATHFINDER_H
#include "DijkstraPq.h"
#include "BFS.h"
class PathFinder {
    private:
    DijkstraPQ dijkstra;
    BFS bfs;
public:
    PathResult findPathDijkstra(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        auto res = dijkstra.dijkstraPQ(n, adj, startNode, endNode);
        res.path = std::move(dijkstra.recover(res.prev, endNode));
        return res;
    }
    PathResult findPathBFS(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        auto res = bfs.runBFS(n, adj, startNode, endNode);
        res.path = std::move(bfs.recover(res.prev, endNode));
        return res;
    }

};
#endif //PATH_FINDING_PATHFINDER_H