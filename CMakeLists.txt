cmake_minimum_required(VERSION  3.31.6)
project(Path_Finding)

set(CMAKE_CXX_STANDARD 20)

find_package(Crow REQUIRED)

add_executable(Path_Finding main.cpp
        Dijkstra.h
        DijkstraPq.h
        Grid.h
        BFS.h
        PathFinder.h
        Util.h
        Server.h
        Astar.h
)

target_link_libraries(Path_Finding Crow::Crow)