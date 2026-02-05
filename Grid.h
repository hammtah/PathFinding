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

/**
 * @brief Compute a unique linear cell identifier from 2D grid coordinates.
 *
 * Maps a 2D grid position (column, row) to a single integer id using:
 *   id = row * width + column
 *
 * @param column Zero-based column index (x).
 * @param row    Zero-based row index (y).
 * @param width  Total number of columns in the grid.
 * @return Linear cell id corresponding to (column, row).
 * @pre 0 <= column < width.
 * @note The validity of @p row depends on the grid height, which is not passed here.
 * @complexity O(1).
 */
int generateCellId(int column, int row, int width) {
    // return y * width + x;
    //35*40+15
    return row * width + column;
}

/**
 * @brief Convert a linear cell identifier back to 2D grid coordinates.
 *
 * Computes the inverse mapping of generateCellId:
 *   x = id % width, y = id / width
 *
 * @param id     Linear cell id.
 * @param width  Total number of columns in the grid (the same value used to build @p id).
 * @return Coord containing zero-based (x, y).
 * @pre width > 0.
 * @complexity O(1).
 */
Coord getCoord(int id, int width) {
    return {id % width, id / width};
}

/**
 * @brief Determine whether a given grid cell is an obstacle.
 *
 * Performs a linear search over the provided obstacle coordinates.
 *
 * @param column Zero-based column index of the queried cell.
 * @param row    Zero-based row index of the queried cell.
 * @param obstacles List of obstacle coordinates (column, row) pairs.
 * @return true if (column, row) exists in @p obstacles; false otherwise.
 * @note Could be optimized by using a boolean grid or a hash-based set for O(1) average lookups.
 * @complexity O(obstacles.size()).
 */
bool isObstacle(int column, int row, std::vector<pii> obstacles) {
    for (auto obstacle : obstacles) {
        if (obstacle.first == column && obstacle.second == row) return true;
    }
    return false;
}

/**
 * @brief Determine whether a given grid cell is a swamp (higher traversal cost).
 *
 * Performs a linear search over the provided swamp coordinates.
 *
 * @param column Zero-based column index of the queried cell.
 * @param row    Zero-based row index of the queried cell.
 * @param swamps List of swamp coordinates (column, row) pairs.
 * @return true if (column, row) exists in @p swamps; false otherwise.
 * @complexity O(swamps.size()).
 */
bool isSwamp(int column, int row, std::vector<pii> swamps) {
    for (auto swamp : swamps) {
        if (swamp.first == column && swamp.second == row) return true;
    }
    return false;
}

/**
 * @brief Add valid 4-directional neighbors of a cell to the adjacency list.
 *
 * For the cell at (column, row), this function:
 *  - Skips neighbors that are outside the grid bounds or are obstacles.
 *  - Adds edges to left, right, up, and down neighbors when passable.
 *  - Assigns edge weight 10 if the neighbor is a swamp; otherwise weight 1.
 *
 * @param column    Zero-based column index of the source cell.
 * @param row       Zero-based row index of the source cell.
 * @param columns   Total number of columns in the grid.
 * @param rows      Total number of rows in the grid.
 * @param obstacles List of obstacle coordinates (column, row) pairs.
 * @param adj       Adjacency list to be mutated; size must be rows * columns.
 * @param swamps    List of swamp coordinates (column, row) pairs.
 * @pre 0 <= column < columns and 0 <= row < rows.
 * @pre adj.size() == static_cast<size_t>(rows * columns).
 * @sideeffects Mutates adj[id] by appending outgoing edges from the source cell.
 * @complexity O(1 + lookups), though lookups are linear in the size of @p obstacles and @p swamps for up to four checks.
 */
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

/**
 * @brief Build a 4-connected adjacency list representation of the grid.
 *
 * Iterates all grid cells and, for passable cells, adds edges to their valid
 * neighbors using addNeighbors. Obstacles are skipped; swamps increase edge cost.
 *
 * @param start     Start cell (column, row). Currently unused.
 * @param end       End cell (column, row). Currently unused.
 * @param obstacles List of obstacle coordinates (column, row) pairs.
 * @param columns   Total number of columns in the grid.
 * @param rows      Total number of rows in the grid.
 * @param swamps    List of swamp coordinates (column, row) pairs.
 * @return Adjacency list with size rows * columns.
 * @note The @p start and @p end parameters are not used in the current implementation
 *       but are kept for API symmetry or future optimizations.
 * @complexity O(rows * columns * (1 + |obstacles| + |swamps|)) with the current linear membership checks.
 */
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