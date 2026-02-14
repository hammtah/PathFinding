//
// Created by taha on 1/30/26.
//

#ifndef PATH_FINDING_DIJKSTRAPQ_H
#define PATH_FINDING_DIJKSTRAPQ_H
#include <vector>
#include <queue>
#include <algorithm>

/**
 * @brief Sentinel distance used to represent "no edge" or "unreachable".
 *
 * This constant is used both for initializing distances in Dijkstra's algorithm
 * and as the missing-edge marker in adjacency matrices.
 * @note Should be strictly greater than any possible finite path length in the problem domain.
 */
const int INF = 1e9;

/**
 * @brief Helper alias for readability: pair<distance, node_id>.
 *
 * Used as the element type in the min-heap (priority_queue) during Dijkstra's algorithm.
 */
typedef std::pair<int, int> pii;

/**
 * @brief Directed edge with an integer weight.
 */
struct Edge {
    int to;      ///< Destination node id (0-based index into the adjacency list).
    int weight;  ///< Non-negative edge weight (cost) for traversing to @ref to.
};

/**
 * @brief Result container for Dijkstra's algorithm and related utilities.
 */
struct PathResult {
    std::vector<int> dist;    ///< dist[v] = shortest known distance from start to v (INF if unreachable).
    std::vector<int> prev;    ///< prev[v] = predecessor of v on a shortest path (-1 if none).
    std::vector<int> visited; ///< Node ids in the order they are popped from the priority queue.
    std::vector<int> path;    ///< Optional path container; not populated by dijkstraPQ(), can be filled via recover().
    float executionTime;      ///< Execution time of the path finding operation in ms
};

/**
 * @brief Dijkstra's shortest path utilities using a priority queue (min-heap).
 *
 * Provides:
 *  - dijkstraPQ: runs Dijkstra on an adjacency list and returns distances, predecessors, and visitation order.
 *  - recover: reconstructs a path from a predecessor array.
 *  - matrixToList: converts an adjacency matrix (using INF for no edge) to an adjacency list.
 *  - shortestPath: convenience function running Dijkstra on a matrix and returning the recovered path.
 */
class DijkstraPQ {
public:
    /**
     * @brief Compute shortest paths from startNode using Dijkstra's algorithm with early exit at endNode.
     *
     * @param n         Number of nodes in the graph (nodes are 0..n-1).
     * @param adj       Adjacency list where adj[u] contains outgoing edges from node u.
     * @param startNode Source node id.
     * @param endNode   Target node id; algorithm stops once this node is popped from the queue.
     * @return PathResult containing:
     *         - dist: shortest distances from startNode,
     *         - prev: predecessor array to reconstruct paths,
     *         - visited: nodes in the order they were finalized (popped).
     *         Note: result.path is not populated by this method.
     * @pre 0 <= startNode < n and 0 <= endNode < n.
     * @pre adj.size() == static_cast<size_t>(n).
     * @pre All edge weights are non-negative (required for Dijkstra's correctness).
     * @complexity O((V + E) log V) with a binary heap, where V = n and E = total number of edges.
     */
    PathResult dijkstraPQ(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);
        std::vector<int> visited;

        // Min-heap: stores pairs of (distance, node)
        // We want the smallest distance at the top
        std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

        //Initialize pq with startNode
        dist[startNode] = 0;
        pq.push({0, startNode});

        while (!pq.empty()) {
            int d = pq.top().first; //Distance
            int node = pq.top().second; //Node id
            pq.pop();

            // skip this old entry if we have already a shortest path to node 'u'
            if (d > dist[node]) continue;

            visited.push_back(node);
            // Break if we just popped the destination
            if (node == endNode) break;
            // Loop over neighbors
            for (auto& edge : adj[node]) {
                int neighbor = edge.to;//Neighbor
                int weight = edge.weight;

                if (dist[node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[node] + weight;
                    prev[neighbor] = node;
                    pq.push({dist[neighbor], neighbor});
                }
            }
        }
        return {dist, prev, visited};
    }

    /**
     * @brief Reconstruct a path ending at endNode by following predecessors.
     *
     * Produces a sequence [start, ..., endNode] by following prev[endNode] -> prev[prev[endNode]] -> ... -> -1.
     *
     * @param prev     Predecessor array where prev[v] is the predecessor of v on a shortest path, or -1 if none.
     * @param endNode  Destination node id to reconstruct to.
     * @return Vector of node ids representing the path. If no path exists under the given @p prev,
     *         the returned vector will contain only endNode.
     * @pre 0 <= endNode < prev.size().
     * @complexity O(L), where L is the number of nodes in the reconstructed path.
     * @note This function does not validate reachability; callers may need to inspect distances to decide if the path is valid.
     */
    std::vector<int> recover(std::vector<int> prev, int endNode) {
        std::vector<int> path;
        for (int i = endNode; i != -1; i = prev[i]) {
            // if (i>=prev.size()) return path;

            path.push_back(i);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    /**
     * @brief Convert an adjacency matrix to an adjacency list(used only for testing).
     *
     * The matrix must be square (n x n). Entries equal to INF indicate no edge.
     * Self-loops with weight 0 are ignored.
     *
     * @param matrix Square matrix of edge weights where matrix[i][j] is the weight from i to j,
     *               or INF if no edge exists.
     * @return Adjacency list representation of the same graph.
     * @complexity O(n^2) to scan the matrix.
     */
    std::vector<std::vector<Edge>> matrixToList(const std::vector<std::vector<int>>& matrix) {
        int n = matrix.size();
        // Initialize a list with 'n' empty vectors
        std::vector<std::vector<Edge>> adj(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // If there is a valid weight and it's not a self-loop with 0
                if (matrix[i][j] != INF && i != j) {
                    // matrix[i][j] is the weight, j is the destination node
                    adj[i].push_back({j, matrix[i][j]});
                }
            }
        }
        return adj;
    }

// public:
    /**
     * @brief Compute a shortest path between two nodes given an adjacency matrix.
     *
     * This is a convenience wrapper that converts @p matrix to an adjacency list,
     * runs Dijkstra's algorithm, and reconstructs a path using the predecessor array.
     *
     * @param matrix    Square adjacency matrix; INF denotes absence of an edge.
     * @param startNode Start node id.
     * @param endNode   End node id.
     * @return Vector of node ids representing a path from startNode to endNode.
     *         If no path exists with the given matrix and INF convention, the vector will contain only endNode.
     * @pre 0 <= startNode, endNode < matrix.size().
     * @warning Consistency with the global INF is required; edges absent must be exactly INF.
     * @complexity O(n^2 + (V + E) log V), with n = matrix.size(), V = n, and E determined by non-INF entries.
     */
    std::vector<int> shortestPath(std::vector<std::vector<int>> matrix, int startNode, int endNode) {
        auto list = matrixToList(matrix);
        return recover(dijkstraPQ(list.size(),list, startNode, endNode).prev, endNode);
    }
};
#endif //PATH_FINDING_DIJKSTRAPQ_H