#include "tests.h"

#include <nestl/algorithm.hpp>
#include <nestl/vector.hpp>
#include <nestl/iterator.hpp>






void copy_success_test()
{
    const size_t size = 7;
    const int input [size] = {0, 1, 2, 3, 4, 5, 6};
    long output [size] = {0};

    auto err = nestl::copy<std::error_condition>(std::begin(input), std::end(input), output);
    ASSERT_SUCCESS_EX(err, "nestl::copy failed with error: " << err.message());
}


void copy_fail_test()
{
    {
        nestl::vector<NonCopyableButAssignCopyable> input;
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));


        nestl::vector<NonCopyableButAssignCopyable> output;
        ASSERT_SUCCESS(output.resize(3));

        NonCopyableButAssignCopyable::return_error_on_assign = true;
        auto err = nestl::copy<std::error_condition>(std::begin(input), std::end(input), output.begin());
        NonCopyableButAssignCopyable::return_error_on_assign = false;

        ASSERT_FAILURE(err);
    }

    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);
}


void copy_success_back_inserter_test()
{
    {
        nestl::vector<NonCopyableButAssignCopyable> input;
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));


        nestl::vector<NonCopyableButAssignCopyable> output;

        ASSERT_SUCCESS(nestl::copy<std::error_condition>(std::begin(input), std::end(input), nestl::back_inserter(output)));
    }

    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);
}

void copy_fail_back_inserter_test()
{
    {
        nestl::vector<NonCopyableButAssignCopyable> input;
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));
        ASSERT_SUCCESS(input.push_back(NonCopyableButAssignCopyable()));


        nestl::vector<NonCopyableButAssignCopyable> output;

        NonCopyableButAssignCopyable::return_error_on_assign = true;
        auto err = nestl::copy<std::error_condition>(std::begin(input), std::end(input), nestl::back_inserter(output));
        NonCopyableButAssignCopyable::return_error_on_assign = false;

        ASSERT_FAILURE(err);
        ASSERT_TRUE(output.empty());
    }

    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);
}




void algorithm_test()
{
    std::cerr << "test of algorithms started" << std::endl;

    copy_success_test();
    copy_fail_test();
    copy_success_back_inserter_test();
    copy_fail_back_inserter_test();

    std::cerr << "test of algorithms finished" << std::endl;
}


