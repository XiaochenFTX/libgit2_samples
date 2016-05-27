cmake_minimum_required(VERSION 3.3)
project(sample10)

set(SOURCE_FILES main.cpp)

add_executable(sample10 ${SOURCE_FILES})
target_link_libraries(sample10 git2)