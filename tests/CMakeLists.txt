project(nestl_tests)

set(nestl_tests_sources
	tests.h
	main.cpp
	vector_test.cpp
	shared_ptr_test.cpp
	algorithm_test.cpp
)

add_executable(nestl_tests ${nestl_tests_sources})
