cmake_minimum_required(VERSION 2.8)

project(nestl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(-std=c++11)


set (nestl_sources
	nestl/nestl.cpp

	nestl/algorithm.hpp
	nestl/allocator.hpp
	nestl/class_traits.hpp
	nestl/config.hpp
	nestl/iterator.hpp
	nestl/memory.hpp
	nestl/noncopyable.hpp
	nestl/shared_ptr.hpp
	nestl/type_traits.hpp
	nestl/vector.hpp
	nestl/operation_error.hpp
)


add_library(nestl_lib ${nestl_sources})

if (EXISTS ${NESTL_GOOGLEMOCK_DIR}/CMakeLists.txt)
  add_subdirectory(tests)
endif()
