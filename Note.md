Add a benchmark (requests, threads, duration...);why you choose crow
[x]add BFS algorithm
Refactor code
add mazes
[x] add swamp with weight > 1 to show difference between dijkstra and bfs
add battle mode : the two algos start at the same time in different colors;If they overlap, it turns Purple.
## Notes
BFS:(//add strategy pattern)
used for unweighted graphs, traverse all nodes level by level starting with the root then its adjacent nodes
the first time we achieve the end node then this is the shortest path

## IMPORTANT : 
Shortest path nodes: this is the path length; the path include the start and end cells.( the same for visited nodes; the start is included) => so we've made in front length()-1
Path weight: weight from start to end (start is excluded) 