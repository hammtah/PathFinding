//
// Created by taha on 1/30/26.
//

#ifndef PATH_FINDING_DIJKSTRAPQ_H
#define PATH_FINDING_DIJKSTRAPQ_H
#include <vector>
#include <queue>
#include <algorithm>

const int INF = 1e9;

// Helper to make the code readable: pair<distance, node_id>
typedef std::pair<int, int> pii;

struct Edge {
    int to;
    int weight;
};
struct DijkstraResult {
    std::vector<int> dist;
    std::vector<int> prev;
};

class DijkstraPQ {
public:

    DijkstraResult dijkstraPQ(int n, const std::vector<std::vector<Edge>>& adj, int startNode) {
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);

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
    std::vector<int> shortestPath(std::vector<std::vector<int>> matrix, int startNode, int endNode) {
        auto list = matrixToList(matrix);
        return recover(dijkstraPQ(list.size(),list, startNode).prev, endNode);
    }
};
#endif //PATH_FINDING_DIJKSTRAPQ_H