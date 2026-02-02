//
// Created by taha on 1/30/26.
//

#ifndef PATH_FINDING_DIJKSTRA_H
#define PATH_FINDING_DIJKSTRA_H

const int INF = 1e9; // Our representation of Infinity

struct PathResult {
    std::vector<int> dist;
    std::vector<int> prev;
};

class Dijkstra {
    PathResult dijkstra(const std::vector<std::vector<int>>& matrix, int startNode) {
        int n = matrix.size();

        // 1. Initialize data structures
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);    // -1 means no parent yet
        std::vector<bool> visited(n, false);

        // Starting point distance is always 0
        dist[startNode] = 0;

        for (int i = 0; i < n; i++) {
            //  Pick the unvisited node with the smallest distance
            int u = -1;
            for (int j = 0; j < n; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }

            // If the smallest distance is still INF, remaining nodes are unreachable
            if (dist[u] == INF) break;

            //  Mark as Visited
            visited[u] = true;

            // Relaxation: Update neighbors of u
            for (int v = 0; v < n; v++) {
                // If there's an edge from u to v
                if (matrix[u][v] != INF) {
                    // If weight(u, v) + dist[u] is better than current dist[v]
                    if (dist[u] + matrix[u][v] < dist[v]) {
                        dist[v] = dist[u] + matrix[u][v];
                        prev[v] = u; // Drop a "breadcrumb"
                    }
                }
            }
        }

        return {dist, prev};
    }

    std::vector<int> recover(std::vector<int> prev, int endNode) {
        std::vector<int> path;
        for (int i = endNode; i != -1; i = prev[i]) {
            path.push_back(i);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    public:
    vector<int> shortestPath(vector<vector<int>> matrix, int startNode, int endNode) {
        return recover(dijkstra(matrix, startNode).prev, endNode);
    }
};
#endif //PATH_FINDING_DIJKSTRA_H