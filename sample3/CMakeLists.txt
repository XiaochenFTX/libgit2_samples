cmake_minimum_required(VERSION 3.3)
project(sample3)

set(SOURCE_FILES main.cpp main.cpp)

add_executable(sample3 ${SOURCE_FILES})
target_link_libraries(sample3 git2)