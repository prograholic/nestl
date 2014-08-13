cmake_minimum_required(VERSION 2.8)

project(nestl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(-std=c++11)

add_subdirectory(tests)
