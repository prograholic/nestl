project(set_test)

set(set_test_sources
    set_test.hpp
    set_test_constructor.cpp
    set_test_insert.cpp
)

nestl_add_simple_test(set_test SOURCES ${set_test_sources})

