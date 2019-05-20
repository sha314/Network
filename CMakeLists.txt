cmake_minimum_required(VERSION 3.0)
project(Network)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS  "-pthread -fopenmp")

set(SOURCE_FILES
        src/main.cpp
        src/nets/network.cpp
        src/nets/network.h
        src/util/printer.h
        src/util/time_tracking.cpp
        src/util/time_tracking.h
        src/percolation/cluster.cpp
        src/percolation/cluster.h
        src/nets/node.cpp
        src/nets/node.h
        src/nets/link.cpp
        src/nets/link.h
        src/types.h
        src/ext_libs/inverse_array.h
        src/percolation/network_percolation.cpp
        src/percolation/network_percolation_v2.cpp
        src/percolation/network_percolation_v3.cpp
        src/test/network_build.cpp
        src/test/network_build.h
        src/test/test_network_percolation.cpp
        src/test/test_network_percolation.h
        src/nets/network_MDA.cpp
        src/nets/network_BA.cpp
        src/percolation/network_percolation.h
        src/percolation/network_percolation_reverse.cpp
        src/percolation/network_percolation_reverse.h
        src/percolation/percolation.h
        src/percolation/network_percolation_explosive.h
        src/percolation/network_percolation_template.h src/percolation/network_percolation_explosive.cpp)

add_executable(networks ${SOURCE_FILES})