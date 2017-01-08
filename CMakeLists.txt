cmake_minimum_required(VERSION 3.0)

project(nestl CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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


set (NESTL_CXX_FEATURES_LIST
    cxx_constexpr
    cxx_alignas
    cxx_alignof
    cxx_decltype
    cxx_noexcept
    cxx_rvalue_references
    cxx_static_assert
    cxx_variadic_templates
    cxx_variadic_macros
)

add_library(nestl_lib ${nestl_sources})
target_compile_features(nestl_lib PUBLIC ${NESTL_CXX_FEATURES_LIST})

if (NOT DEFINED NESTL_GOOGLEMOCK_DIR)
  #helper for travis
  set(NESTL_GOOGLEMOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gmock-1.7.0)
endif()

if (EXISTS ${NESTL_GOOGLEMOCK_DIR}/CMakeLists.txt)
  enable_testing()
  add_subdirectory(tests)
endif()
