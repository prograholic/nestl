cmake_minimum_required(VERSION 3.0)

project(nestl CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (nestl_common_headers
    nestl/algorithm.hpp
    nestl/alignment.hpp
    nestl/allocator.hpp
    nestl/allocator_traits.hpp
    nestl/class_operations.hpp
    nestl/config.hpp
    nestl/exception_support.hpp
    nestl/list.hpp
    nestl/memory.hpp
    nestl/operation_error.hpp
    nestl/set.hpp
    nestl/shared_ptr.hpp
    nestl/string.hpp
    nestl/vector.hpp
)

set (nestl_detail_headers
    nestl/detail/msvc.hpp
    nestl/detail/gcc.hpp
    nestl/detail/clang.hpp
    nestl/detail/construct.hpp
    nestl/detail/red_black_tree.hpp
    nestl/detail/select_type.hpp
)

set (nestl_no_exceptions_headers
    nestl/no_exceptions/operation_error.hpp
    nestl/no_exceptions/allocator.hpp
    nestl/no_exceptions/allocator_traits.hpp
)

set (nestl_has_exceptions_headers
    nestl/has_exceptions/operation_error.hpp
    nestl/has_exceptions/allocator.hpp
    nestl/has_exceptions/allocator_traits.hpp
)

set (nestl_sources
    nestl/nestl.cpp

    ${nestl_common_headers}
    ${nestl_detail_headers}
    ${nestl_no_exceptions_headers}
    ${nestl_has_exceptions_headers}
)

source_group(nestl FILES ${nestl_common_headers})
source_group(nestl\\detail FILES ${nestl_detail_headers})
source_group(nestl\\no_exceptions FILES ${nestl_no_exceptions_headers})
source_group(nestl\\has_exceptions FILES ${nestl_has_exceptions_headers})


add_library(nestl_lib ${nestl_sources})

enable_testing()
add_subdirectory(tests)
