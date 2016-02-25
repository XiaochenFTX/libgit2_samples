cmake_minimum_required(VERSION 3.3)
project(sample1)

set(SOURCE_FILES main.cpp)

add_executable(sample1 ${SOURCE_FILES})
target_link_libraries(sample1 git2)