

function (nestl_add_simple_test test_name)
  add_executable(${test_name}_exe ${test_name}.cpp)

  add_test(NAME
           ${test_name}_runner
           COMMAND
           ${test_name}_exe)
endfunction()


add_subdirectory(vector)
add_subdirectory(list)
add_subdirectory(shared_ptr)
add_subdirectory(set)
