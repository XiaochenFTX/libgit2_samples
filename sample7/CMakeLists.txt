cmake_minimum_required(VERSION 3.3)
project(sample7)

set(SOURCE_FILES main.cpp)

add_executable(sample7 ${SOURCE_FILES})
target_link_libraries(sample7 git2)