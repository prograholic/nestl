cmake_minimum_required(VERSION 3.0)

project(nestl CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
    nestl/list.hpp
    nestl/memory.hpp
    nestl/move.hpp
    nestl/noncopyable.hpp
    nestl/numeric_limits.hpp
    nestl/operation_error.hpp
    nestl/set.hpp
    nestl/shared_ptr.hpp
    nestl/static_assert.hpp
    nestl/string.hpp
    nestl/system_error.hpp
    nestl/swap.hpp
    nestl/vector.hpp
)


add_library(nestl_lib ${nestl_sources})

enable_testing()
add_subdirectory(tests)
