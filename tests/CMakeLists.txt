
include(CMakeParseArguments)

function (nestl_add_simple_test test_name)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES)

    cmake_parse_arguments(NESTL_SIMPLE_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    file(WRITE  ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp "#include \"tests/nestl_test.hpp\"\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp "int main() {NESTL_RUN_ALL_TESTS();}\n")


#############

    source_group("Generated Files" FILES ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp)
    add_executable(${test_name}_exe ${NESTL_SIMPLE_TEST_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp)
    enable_exception_support(${test_name}_exe)
    add_test(NAME ${test_name}_runner COMMAND ${test_name}_exe)

#############

    source_group("Generated Files" FILES ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp)
    add_executable(${test_name}_nx_exe ${NESTL_SIMPLE_TEST_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_main.cpp)
    disable_exception_support(${test_name}_nx_exe)
    add_test(NAME ${test_name}_nx_runner COMMAND ${test_name}_nx_exe)


    set_property(TARGET ${test_name}_exe    PROPERTY FOLDER "test/${test_name}")
    set_property(TARGET ${test_name}_nx_exe PROPERTY FOLDER "test/${test_name}")
endfunction()



add_subdirectory(vector)
add_subdirectory(list)
add_subdirectory(shared_ptr)
add_subdirectory(set)
add_subdirectory(class_operations)

