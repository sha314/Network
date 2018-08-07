cmake_minimum_required(VERSION 3.0)
project(Network)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        src/network.cpp
        src/network.h
        src/util/printer.h
        src/util/time_tracking.cpp
        src/util/time_tracking.h
        )

add_executable(network ${SOURCE_FILES})