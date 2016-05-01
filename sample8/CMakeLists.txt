cmake_minimum_required(VERSION 3.3)
project(sample8)

set(SOURCE_FILES main.cpp)

add_executable(sample8 ${SOURCE_FILES})
target_link_libraries(sample8 git2)