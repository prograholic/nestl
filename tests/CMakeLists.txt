
if (MSVC)
  # msvc-2012 lacks variadic template support
  if (MSVC_VERSION EQUAL 1700) # mcvc-2012
    message(STATUS "msvc lacks variadic template support")
    add_definitions(-D_VARIADIC_MAX=10)   
  endif()
endif(MSVC)


add_subdirectory(${NESTL_GOOGLEMOCK_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googlemock)

include_directories(${NESTL_GOOGLEMOCK_DIR}/include)
include_directories(${NESTL_GOOGLEMOCK_DIR}/gtest/include)


config_compiler_and_linker()

if (NESTL_DISABLE_STD)
  add_definitions(-DNESTL_PLATFORM_HEADER="tests/sample_test_platform.hpp")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-nostdinc)
	add_compile_options(-nostdinc++)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(/X)
  endif()
endif()

function(cxx_test2 name libs)
  cxx_test_with_flags("${name}" "${cxx_default}" "${libs}"
	"${name}.cpp" ${ARGN})
endfunction()


add_subdirectory(vector)
add_subdirectory(list)
add_subdirectory(shared_ptr)
