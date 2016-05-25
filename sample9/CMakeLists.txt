cmake_minimum_required(VERSION 3.3)
project(sample9)

set(SOURCE_FILES main.cpp)

add_executable(sample9 ${SOURCE_FILES})
target_link_libraries(sample9 git2)