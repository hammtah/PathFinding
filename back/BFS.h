//
// Created by taha on 2/2/26.
//

#ifndef PATH_FINDING_BFS_H
#define PATH_FINDING_BFS_H
// #include "DijkstraPq.h"
class BFS {
    public:
    PathResult runBFS(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode) {
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);
        std::vector<int> visited;

        //Stores the nodes ids
        std::queue<int> queue;

        //Initialize queue with startNode
        dist[startNode] = 0;
        queue.push(startNode);

        while (!queue.empty()) {
            int node = queue.front(); //Node id
            queue.pop();

            visited.push_back(node);
            // Break if we just popped the destination
            if (node == endNode) break;
            // Loop over neighbors
            for (auto& edge : adj[node]) {
                int neighbor = edge.to;//Neighbor

                if (dist[neighbor] == INF) {
                    dist[neighbor] = dist[node] + 1;
                    prev[neighbor] = node;
                    queue.push(neighbor);
                }
            }
        }
        return {dist, prev, visited};
    }

    std::vector<int> recover(std::vector<int> prev, int endNode) {
        std::vector<int> path;
        for (int i = endNode; i != -1; i = prev[i]) {
            // if (i>=prev.size()) return path;

            path.push_back(i);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

};
#endif //PATH_FINDING_BFS_H