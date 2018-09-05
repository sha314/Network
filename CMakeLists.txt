cmake_minimum_required(VERSION 3.0)
project(Network)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS  "-pthread -fopenmp")

set(SOURCE_FILES
        main.cpp
        src/nets/network.cpp
        src/nets/network.h
        src/util/printer.h
        src/util/time_tracking.cpp
        src/util/time_tracking.h
        src/cluster.cpp
        src/cluster.h
        src/nets/node.cpp
        src/nets/node.h
        src/nets/link.cpp
        src/nets/link.h
        src/types.h
        src/ext_libs/inverse_array.h
        src/nets/network_percolation.cpp)

add_executable(networks ${SOURCE_FILES})