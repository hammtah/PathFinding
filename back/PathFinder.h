//
// Created by taha on 2/2/26.
//

#ifndef PATH_FINDING_PATHFINDER_H
#define PATH_FINDING_PATHFINDER_H
#include "Util.h"
#include "DijkstraPq.h"
#include "BFS.h"
#include "Astar.h"
#include <chrono>
/**
 * @brief Facade for running different path-finding algorithms.
 *
 * Provides convenience wrappers that execute an algorithm on an adjacency list,
 * then reconstruct and attach the path to the returned PathResult.
 */
class PathFinder {
    private:
    DijkstraPQ dijkstra; ///< Dijkstra's algorithm runner.
    BFS bfs;             ///< Breadth-first search runner (unweighted shortest path).
    Astar astar;         ///< A* algorithm runner.

public:
    /**
     * @brief Compute shortest path using Dijkstra's algorithm.
     *
     * @param n         Number of nodes in the graph.
     * @param adj       Adjacency list where adj[u] contains outgoing edges for u.
     * @param startNode Source node id.
     * @param endNode   Target node id.
     * @return PathResult with distances, predecessors, visited order, and recovered path.
     * @pre 0 <= startNode, endNode < n; adj.size() == n; non-negative edge weights.
     * @complexity O((V + E) log V) with a binary heap, V = n.
     */
    PathResult findPathDijkstra(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        PathResult res;
        float duration = 0;
        {
            Util::Timer t(&duration);
            res = std::move(dijkstra.dijkstraPQ(n, adj, startNode, endNode));
        }
        res.path = std::move(dijkstra.recover(res.prev, endNode));
        res.executionTime = duration;
        return res;
    }

    /**
     * @brief Compute shortest path using BFS (for unweighted graphs).
     *
     * @param n         Number of nodes in the graph.
     * @param adj       Adjacency list.
     * @param startNode Source node id.
     * @param endNode   Target node id.
     * @return PathResult with predecessors, visited order, and recovered path. dist contains hop counts.
     * @pre 0 <= startNode, endNode < n; edges implicitly have uniform weight.
     * @complexity O(V + E).
     */
    PathResult findPathBFS(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        PathResult res;
        float duration = 0;
        {
            Util::Timer t(&duration);
            res = std::move(bfs.runBFS(n, adj, startNode, endNode));
        }
        res.path = std::move(bfs.recover(res.prev, endNode));
        res.executionTime = duration;
        return res;
    }

    /**
     * @brief Compute shortest path using A* search on a grid-based graph.
     *
     * @param n         Number of nodes.
     * @param adj       Adjacency list.
     * @param startNode Start node id.
     * @param endNode   End node id.
     * @param width     Grid width used for heuristic coordinate mapping.
     * @return PathResult with distances, predecessors, visited order, and recovered path.
     * @pre 0 <= startNode, endNode < n; consistent heuristic usage inside A*; non-negative weights.
     * @complexity Depends on heuristic quality; worst-case similar to Dijkstra: O((V + E) log V).
     */
    PathResult findPathAstar(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode, int width) {
        PathResult res;
        float duration = 0;
        {
            Util::Timer t(&duration);
            res = std::move(astar.aStarPQ(n, adj, startNode, endNode, width));
        }
        res.path = std::move(bfs.recover(res.prev, endNode));
        res.executionTime = duration;
        return res;
    }

};
#endif //PATH_FINDING_PATHFINDER_H