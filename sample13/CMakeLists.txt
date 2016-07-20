cmake_minimum_required(VERSION 3.3)
project(sample13)

set(SOURCE_FILES main.cpp)

add_executable(sample13 ${SOURCE_FILES})
target_link_libraries(sample13 git2)