cmake_minimum_required(VERSION 4.0)
project(Path_Finding)

set(CMAKE_CXX_STANDARD 20)

add_executable(Path_Finding main.cpp
        Dijkstra.h
        DijkstraPq.h
        Grid.h)
