cmake_minimum_required(VERSION 3.3)
project(sample12)

set(SOURCE_FILES main.cpp)

add_executable(sample12 ${SOURCE_FILES})
target_link_libraries(sample12 git2)