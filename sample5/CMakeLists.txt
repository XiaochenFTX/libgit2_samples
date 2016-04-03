cmake_minimum_required(VERSION 3.3)
project(sample5)

set(SOURCE_FILES main.cpp)

add_executable(sample5 ${SOURCE_FILES})
target_link_libraries(sample5 git2)