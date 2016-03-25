cmake_minimum_required(VERSION 3.3)
project(sample4)

set(SOURCE_FILES main.cpp)

add_executable(sample4 ${SOURCE_FILES})
target_link_libraries(sample4 git2)