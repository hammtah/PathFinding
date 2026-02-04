//
// Created by taha on 2/4/26.
//

#ifndef PATH_FINDING_ASTAR_H
#define PATH_FINDING_ASTAR_H
// Manhattan Distance Heuristic
class Astar{

int calculateHeuristic(int node, int endNode, int width) {
    int x1 = node % width;
    int y1 = node / width;
    int x2 = endNode % width;
    int y2 = endNode / width;
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}
public:
PathResult aStarPQ(int n, const std::vector<std::vector<Edge>>& adj, int startNode, int endNode, int width) {
    std::vector<int> gScore(n, INF); // Actual distance from start
    std::vector<int> prev(n, -1);
    std::vector<int> visited;

    // Min-heap: stores pairs of (fScore, node_id)
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    gScore[startNode] = 0;

    // For start node, fScore = gScore + hScore
    int fStart = 0 + calculateHeuristic(startNode, endNode, width);
    pq.push({fStart, startNode});

    while (!pq.empty()) {
        int f = pq.top().first; // Current fScore
        int node = pq.top().second;
        pq.pop();

        // Important: A* can push the same node multiple times with different fScores.
        // We only care about the first time we POP it.
        bool alreadyVisited = false;
        for(int v : visited) if(v == node) { alreadyVisited = true; break; }
        if (alreadyVisited) continue;

        visited.push_back(node);

        if (node == endNode) break;

        for (auto& edge : adj[node]) {
            int neighbor = edge.to;
            int weight = edge.weight;

            int tentative_gScore = gScore[node] + weight;

            if (tentative_gScore < gScore[neighbor]) {
                prev[neighbor] = node;
                gScore[neighbor] = tentative_gScore;

                // fScore = gScore + heuristic
                int fScore = gScore[neighbor] + calculateHeuristic(neighbor, endNode, width);
                pq.push({fScore, neighbor});
            }
        }
    }
    // We reuse 'gScore' as 'dist' in the return struct
    return {gScore, prev, visited};
}
};
#endif //PATH_FINDING_ASTAR_H
