cmake_minimum_required(VERSION 3.0)
project(Network)

### for pgi
#SET(CMAKE_CXX_COMPILER /home/shahnoor/software/pgi/linux86-64/19.4/bin/pgc++)
#SET(CMAKE_C_COMPILER /home/shahnoor/software/pgi/linux86-64/19.4/bin/pgcc)
#SET(CMAKE_CXX_FLAGS  "-O3 std=c++11")


## for gcc
set(CMAKE_CXX_STANDARD 11)

#SET(CMAKE_CXX_FLAGS  "-Wall") # for debug without flag
SET(CMAKE_CXX_FLAGS  "-Wall -D DEBUG_FLAG") # for debug only
## if -O3 is enabled during compilation then step by step debugging is harder
#SET(CMAKE_CXX_FLAGS  "-O3 -Wall -D DEBUG_FLAG")
#SET(CMAKE_CXX_FLAGS  "-O3 -Wall") # Only Release flag
#SET(CMAKE_CXX_FLAGS  "-pthread -fopenmp -O3 -Wall") # Only Release flag


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
        src/percolation/explosive/network_percolation_explosive_v3.cpp
        src/percolation/explosive/network_percolation_explosive_inverted.cpp
        src/percolation/explosive/network_percolation_explosive_inverted.h
        src/test/run_network_ba.cpp
        src/test/run_network_ba.h
        src/percolation/network_percolation_v6.cpp
        src/percolation/network_percolation_v6.h
        src/percolation/explosive/network_percolation_explosive_v6.cpp
        src/percolation/explosive/network_percolation_explosive_v6.h
        src/nets/ER/network_ER.cpp
        src/nets/ER/network_ER.h
        src/nets/ER/test.cpp
        src/nets/ER/test.h
        src/percolation/network_percolation_v7.cpp
        src/percolation/network_percolation_v7.h src/nets/MDA/network_MDA.h src/nets/BA/network_BA.h src/nets/BA/network_BA_v7.cpp src/nets/BA/network_BA_v7.h src/nets/MDA/network_MDA_v7.cpp src/nets/MDA/network_MDA_v7.h)

add_executable(networks ${SOURCE_FILES})