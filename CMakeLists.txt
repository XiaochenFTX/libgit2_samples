cmake_minimum_required(VERSION 3.3)
project(libgit2_samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(extras/libgit2/include)

#fixed : ld: library not found for -lssh2
LINK_DIRECTORIES(${LIBSSH2_LIBRARY_DIRS})

add_subdirectory(extras/libgit2)
add_subdirectory(sample1)
add_subdirectory(sample2)
add_subdirectory(sample3)
add_subdirectory(sample4)
add_subdirectory(sample5)
add_subdirectory(sample6)
add_subdirectory(sample7)