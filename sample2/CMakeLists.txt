cmake_minimum_required(VERSION 3.3)
project(sample2)

set(SOURCE_FILES main.cpp)

add_executable(sample2 ${SOURCE_FILES})
target_link_libraries(sample2 git2)