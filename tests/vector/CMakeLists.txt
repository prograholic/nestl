project(vector_test)


set(vector_test_sources
    vector_test.cpp
    vector_test_constructor.cpp
    vector_test_assign.cpp
)

nestl_add_simple_test(vector_test SOURCES ${vector_test_sources})

