
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


# set highest possible warning level, also treat warning as error
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Werror)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)

  if (NESTL_ENABLE_CXX11)
	# we cannot use pedantic for C++-03 mode, because we use some compiler-specific features
	# for emulating C++-11 features
	add_compile_options(-pedantic)
  else (NESTL_ENABLE_CXX11)
	# variadic macros are very useful, so we use them anyway
	add_compile_options(-Wno-variadic-macros)
  endif (NESTL_ENABLE_CXX11)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/W4)
  add_compile_options(/WX)
endif()


if (NESTL_DISABLE_STD)
  add_definitions(-DNESTL_PLATFORM_HEADER="tests/sample_test_platform.hpp")
endif()

function(cxx_test2 name libs)
  cxx_test_with_flags("${name}" "${cxx_default}" "${libs}"
	"${name}.cpp" ${ARGN})
endfunction()


add_subdirectory(vector)
add_subdirectory(list)
add_subdirectory(shared_ptr)
add_subdirectory(set)
add_subdirectory(numeric)
