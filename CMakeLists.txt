cmake_minimum_required(VERSION 2.8)

project(nestl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
else()
endif()

add_subdirectory(tests)
