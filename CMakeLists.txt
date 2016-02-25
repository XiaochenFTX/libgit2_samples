cmake_minimum_required(VERSION 3.3)
project(libgit2_samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(extras/libgit2/include)
add_subdirectory(extras/libgit2)
add_subdirectory(sample1)