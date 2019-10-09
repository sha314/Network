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
        src/percolation/cluster/cluster.cpp
        src/percolation/cluster/cluster.h
        src/percolation/cluster/cluster_v2.cpp
        src/percolation/cluster/cluster_v3.cpp
        src/percolation/cluster/cluster_v2.h
        src/percolation/cluster/cluster_v3.h
        src/nets/node/node.cpp
        src/nets/node/node.h
        src/nets/link/link.cpp
        src/nets/link/link.h
        src/types.h
#        src/ext_libs/inverse_array.h
        src/test/network_build.cpp
        src/test/network_build.h
        src/test/test_network_percolation.cpp
        src/test/test_network_percolation.h
        src/nets/MDA/network_MDA.cpp
        src/nets/BA/network_BA.cpp
        src/percolation/network_percolation.h
        src/percolation/reverse/network_percolation_reverse.cpp
        src/percolation/reverse/network_percolation_reverse.h
        src/percolation/percolation.h
        src/percolation/explosive/network_percolation_explosive.h
        src/percolation/network_percolation_template.h
        src/percolation/explosive/network_percolation_explosive.cpp
        src/percolation/network_percolation.cpp
        src/percolation/network_percolation_v3.cpp
        src/percolation/network_percolation_v3.h
        src/percolation/network_percolation_v2.cpp
        src/percolation/network_percolation_v2.h
        src/percolation/network_percolation_v4.cpp
        src/percolation/network_percolation_v4.h
        src/percolation/network_percolation_v5.cpp
        src/percolation/network_percolation_v5.h
        src/nets/BA/network_BA_v2.cpp
        src/nets/BA/network_BA_v2.h
        src/test/test_network_ba_v2.cpp
        src/test/test_network_ba_v2.h
        src/percolation/explosive/network_percolation_explosive_v5.cpp
        src/percolation/explosive/network_percolation_explosive_v5.h
        src/percolation/explosive/network_percolation_explosive_v3.cpp src/percolation/explosive/network_percolation_explosive_inverted.cpp src/percolation/explosive/network_percolation_explosive_inverted.h src/test/run_network_ba.cpp src/test/run_network_ba.h)

add_executable(networks ${SOURCE_FILES})