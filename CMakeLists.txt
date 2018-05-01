cmake_minimum_required(VERSION 2.8)
project(Darwin_trader)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

add_executable(Darwin_trader main.cpp gene_functions_H.h gene_resources.cpp random_range.h random_range.cpp main.h)

target_link_libraries (Darwin_trader ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")