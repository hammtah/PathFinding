//
// Created by LLM on 3/24/26.
//

#ifndef PATH_FINDING_MAZE_H
#define PATH_FINDING_MAZE_H

#include <vector>
#include <optional>
#include <random>
#include <algorithm>
#include "DijkstraPq.h" // for pii alias

/**
 * @brief Generate a perfect maze as a set of obstacle coordinates.
 *
 * Uses randomized depth-first search (recursive backtracker) to carve passages on a grid.
 * Cells at odd coordinates (x, y) are treated as potential passages, even coordinates are walls.
 *
 * @param width   Grid width (columns).
 * @param height  Grid height (rows).
 * @param start   Optional start coordinate to ensure is open.
 * @param end     Optional end coordinate to ensure is open.
 * @return Vector of obstacle coordinates (x, y) that represent walls.
 */
inline std::vector<pii> generateMazeObstacles(int width, int height,
                                              std::optional<pii> start = std::nullopt,
                                              std::optional<pii> end = std::nullopt) {
    std::vector<pii> obstacles;
    if (width <= 2 || height <= 2) {
        // Too small to form a meaningful maze
        return obstacles;
    }

    // 1 = wall (obstacle), 0 = passage
    std::vector<std::vector<int>> maze(height, std::vector<int>(width, 1));
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    std::mt19937 rng(std::random_device{}());

    auto randOdd = [&](int max) -> int {
        // returns an odd index in [1, max-2]
        if (max < 3) return 1;
        int count = (max - 2) / 2; // number of odd positions
        int k = std::uniform_int_distribution<int>(0, std::max(0, count - 1))(rng);
        return 1 + 2 * k;
    };

    // Choose a random odd starting cell
    int sx = randOdd(width);
    int sy = randOdd(height);

    // Iterative DFS using a stack of odd cells
    std::vector<pii> st;
    st.emplace_back(sx, sy);
    visited[sy][sx] = true;
    maze[sy][sx] = 0;

    while (!st.empty()) {
        auto [cx, cy] = st.back();

        // Shuffle directions of 2-cell steps (to land on odd coords)
        std::vector<pii> dirs = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}};
        std::shuffle(dirs.begin(), dirs.end(), rng);

        bool carved = false;
        for (auto [dx, dy] : dirs) {
            int nx = cx + dx;
            int ny = cy + dy;
            // keep within inner border so that nx,ny are valid odd cells
            if (nx <= 0 || nx >= width - 1 || ny <= 0 || ny >= height - 1) continue;
            if (!visited[ny][nx]) {
                // Carve passage between current and neighbor
                maze[cy + dy / 2][cx + dx / 2] = 0; // remove the wall between
                maze[ny][nx] = 0;                   // neighbor cell
                visited[ny][nx] = true;
                st.emplace_back(nx, ny);
                carved = true;
                break; // continue deepening
            }
        }
        if (!carved) {
            st.pop_back();
        }
    }

    // Ensure start/end are open if provided
    auto openIfProvided = [&](const std::optional<pii>& p) {
        if (!p) return;
        int x = p->first, y = p->second;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            maze[y][x] = 0;
        }
    };
    openIfProvided(start);
    openIfProvided(end);

    // Collect walls as obstacles, excluding forced-open start/end if any
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool isStart = start && start->first == x && start->second == y;
            bool isEnd = end && end->first == x && end->second == y;
            if (maze[y][x] == 1 && !isStart && !isEnd) {
                obstacles.emplace_back(x, y);
            }
        }
    }
    return obstacles;
}

#endif // PATH_FINDING_MAZE_H
