cmake_minimum_required(VERSION 3.3)
project(sample6)

set(SOURCE_FILES main.cpp)

add_executable(sample6 ${SOURCE_FILES})
target_link_libraries(sample6 git2)