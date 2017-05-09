cmake_minimum_required(VERSION 3.0)

project(nestl CXX)

set (CMAKE_MODULE_PATH
    ${nestl_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(disable_exceptions)


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
    nestl/default_operation_error.hpp
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
    nestl/no_exceptions/errc_based_error.hpp
    nestl/no_exceptions/allocator.hpp
)

set (nestl_no_exceptions_details_headers
    nestl/no_exceptions/detail/allocator_traits_construct_helper.hpp
)

set (nestl_has_exceptions_headers
    nestl/has_exceptions/exception_ptr_error.hpp
    nestl/has_exceptions/allocator.hpp
)

set (nestl_has_exceptions_details_headers
    nestl/has_exceptions/detail/allocator_traits_construct_helper.hpp
)

set (nestl_sources
    nestl/nestl.cpp

    ${nestl_common_headers}
    ${nestl_detail_headers}
    ${nestl_no_exceptions_headers}
    ${nestl_no_exceptions_details_headers}
    ${nestl_has_exceptions_headers}
    ${nestl_has_exceptions_details_headers}
)

source_group(nestl FILES ${nestl_common_headers})
source_group(nestl\\detail FILES ${nestl_detail_headers})

source_group(nestl\\no_exceptions FILES ${nestl_no_exceptions_headers})
source_group(nestl\\no_exceptions\\details FILES ${nestl_no_exceptions_details_headers})

source_group(nestl\\has_exceptions FILES ${nestl_has_exceptions_headers})
source_group(nestl\\has_exceptions\\details FILES ${nestl_has_exceptions_details_headers})


add_library(nestl_lib ${nestl_sources})

enable_testing()
add_subdirectory(tests)
