cmake_minimum_required(VERSION 3.0)

project(nestl CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if (NESTL_CONFIG_HAS_CXX11)
  message(STATUS "Using nestl with c++ 11 features")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # fix for g++-4.6 which does not accept -std=c++11 parameter
    add_compile_options(-std=c++0x)
  endif()
else()
  message(STATUS "Using nestl with c++ 03 features")
endif()

if (NESTL_DISABLE_STD)
  message(STATUS "Using nestl without standard library")
  add_definitions(-DNESTL_NO_STD)
else()
  message(STATUS "Using nestl with standard library")
endif()

set (nestl_sources
    nestl/nestl.cpp

    nestl/addressof.hpp
    nestl/algorithm.hpp
    nestl/alignment.hpp
    nestl/allocator.hpp
    nestl/allocator_traits.hpp
    nestl/assert.hpp
    nestl/char_traits.hpp
    nestl/class_traits.hpp
    nestl/config.hpp
    nestl/declval.hpp
    nestl/initializer_list.hpp
    nestl/iterator.hpp
    nestl/list.hpp
    nestl/memory.hpp
    nestl/move.hpp
    nestl/noncopyable.hpp
    nestl/numeric_limits.hpp
    nestl/operation_error.hpp
    nestl/shared_ptr.hpp
    nestl/static_assert.hpp
    nestl/string.hpp
    nestl/system_error.hpp
    nestl/swap.hpp
    nestl/type_traits.hpp
    nestl/vector.hpp
)


add_library(nestl_lib ${nestl_sources})

if (EXISTS ${NESTL_GOOGLEMOCK_DIR}/CMakeLists.txt)
  enable_testing()
  add_subdirectory(tests)
endif()
