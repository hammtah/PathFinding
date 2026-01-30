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
    auto res = dijkstra_pq.dijkstraPQ(a.size(), a, startId);
   printPath(dijkstra_pq.recover(res.prev,generateCellId(end.first, end.second, width)));
    return 0;
}

#include "crow.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello, Crow!";
    });

    app.port(18080).multithreaded().run();
}