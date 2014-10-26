

add_subdirectory(${NESTL_GOOGLEMOCK_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googlemock)

include_directories(${NESTL_GOOGLEMOCK_DIR}/include)
include_directories(${NESTL_GOOGLEMOCK_DIR}/gtest/include)


config_compiler_and_linker()

if (NESTL_DISABLE_STD)
  add_definitions(-DNESTL_PLATFORM_HEADER="tests/sample_test_platform.hpp")
  if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-nostdinc)
    add_compile_options(-nostdinc++)
  else()
    # assume MSVC compiler
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
