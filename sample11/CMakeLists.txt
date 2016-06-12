cmake_minimum_required(VERSION 3.3)
project(sample11)

set(SOURCE_FILES main.cpp)

add_executable(sample11 ${SOURCE_FILES})
target_link_libraries(sample11 git2)