project(shared_ptr_test)

set(shared_ptr_test_sources
    shared_ptr_test.cpp
    shared_ptr_test.hpp
)

nestl_add_simple_test(shared_ptr_test SOURCES ${shared_ptr_test_sources})

