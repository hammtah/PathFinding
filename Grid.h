//
// Created by taha on 1/30/26.
//
#ifndef PATH_FINDING_GRID_H
#define PATH_FINDING_GRID_H
#include <vector>

#include "DijkstraPq.h"

struct Coord {
    int x;
    int y;
};

int generateCellId(int column, int row, int width) {
    // return y * width + x;
    //35*40+15
    return row * width + column;
}
Coord getCoord(int id, int width) {
    return {id % width, id / width};
}
//should be optimized (use array of booleans)
bool isObstacle(int column, int row, std::vector<pii> obstacles) {
    for (auto obstacle : obstacles) {
        if (obstacle.first == column && obstacle.second == row) return true;
    }
    return false;
}

bool isSwamp(int column, int row, std::vector<pii> swamps) {
    for (auto swamp : swamps) {
        if (swamp.first == column && swamp.second == row) return true;
    }
    return false;
}

void addNeighbors(int column, int row, int columns, int rows,std::vector<pii> obstacles, std::vector<std::vector<Edge>>& adj, std::vector<pii> swamps) {
    //generate cell id
    int id = generateCellId(column, row, columns);
    //generate cell's neighbors id
    int n1 = generateCellId(column - 1, row, columns);
    int n2 = generateCellId(column + 1, row, columns);
    int n3 = generateCellId(column, row - 1, columns);
    int n4 = generateCellId(column, row + 1, columns);
    //if neighbors are not obstacles then push them to node's neighbors if their ids are valid
    if ((column-1 >= 0) && !isObstacle(column-1, row, obstacles)) {
        if (isSwamp(column-1,row, swamps)) {
            adj[id].push_back({n1, 10});
        }else {
            adj[id].push_back({n1, 1});
        }
    }
    if ((column+1 < columns) && !isObstacle(column+1,row, obstacles)) {
        if (isSwamp(column+1,row, swamps)) {
            adj[id].push_back({n2, 10});
        }else {
            adj[id].push_back({n2, 1});
        }
    }
    if ((row - 1 >= 0) && !isObstacle(column, row-1, obstacles)) {
        if (isSwamp(column,row-1, swamps)) {
            adj[id].push_back({n3, 10});
        }else {
            adj[id].push_back({n3, 1});
        }

    }
    if ((row+1 < rows) && !isObstacle(column, row+1, obstacles)) {
        if (isSwamp(column,row+1, swamps)) {
            adj[id].push_back({n4, 10});
        }else {
            adj[id].push_back({n4, 1});
        }
    }

}
std::vector<std::vector<Edge>> gridToList(pii start, pii end, std::vector<pii> obstacles, int columns, int rows, std::vector<pii> swamps) {
    //create adjacency list with given size (rows*columns-obstacles)
    std::vector<std::vector<Edge>> adj(rows * columns);
    //loop over cells
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            // if (isObstacle(c,r,obstacles)) continue;
            if (!isObstacle(c,r,obstacles)) {
                // std::cout << "obstacle at " << c << " " << r << std::endl;
                addNeighbors(c, r, columns, rows, obstacles, adj, swamps);
            }
            //generate cell's neighbors id
            //if neighbors are not obstacles then push them to node's neighbors if their ids are valid
            // addNeighbors(c, r, columns, rows, obstacles, adj);
        }
    }
    return adj;
}
#endif //PATH_FINDING_GRID_H