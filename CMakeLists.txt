cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 11)

add_executable(project main.cpp
        Parser.cpp
        Parser.h
        Menu.cpp
        Menu.h
        graph/Graph.h
        graph/Graph.cpp
        Algorithms/Algorithms.h
        Algorithms/Algorithms.cpp)
