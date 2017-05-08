#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{

void run_tests()
{
    {
        nestl::vector<int> vec;

        CheckVectorSize(vec, 0);
    }

    {
        nestl::allocator<int> a;
        nestl::vector<int> vec(a);

        CheckVectorSize(vec, 0);
    }

    {
        nestl::vector<int> vec1;

        nestl::vector<int> vec2(std::move(vec1));

        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }

    {
        nestl::vector<int> vec1;

        ASSERT_OPERATION_SUCCESS(vec1.assign_nothrow(_, 10));

        nestl::vector<int> vec2(std::move(vec1));

        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }


    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with zero allocator
    ////////////////////////////////////////////////////////////////////////////////
    {
        nestl::vector<int, zero_allocator<int> > vec;

        CheckVectorSize(vec, 0);
    }

    {
        zero_allocator<int> a;
        nestl::vector<int, zero_allocator<int> > vec(a);

        CheckVectorSize(vec, 0);
    }

    {
        nestl::vector<int, zero_allocator<int> > vec1;

        nestl::vector<int, zero_allocator<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }


    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with minimal allocator
    ////////////////////////////////////////////////////////////////////////////////
    {
        nestl::vector<int, minimal_allocator<int> > vec;

        CheckVectorSize(vec, 0);
    }

    {
        minimal_allocator<int> a;
        nestl::vector<int, minimal_allocator<int> > vec(a);

        CheckVectorSize(vec, 0);
    }

    {
        nestl::vector<int, minimal_allocator<int> > vec1;

        nestl::vector<int, minimal_allocator<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }

    {
        nestl::vector<int, minimal_allocator<int> > vec1;

        ASSERT_OPERATION_SUCCESS(vec1.assign_nothrow(_, 10));

		nestl::vector<int, minimal_allocator<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }

    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with stateful allocator
    ////////////////////////////////////////////////////////////////////////////////

    {
        nestl::vector<int, allocator_with_state<int> > vec;

        CheckVectorSize(vec, 0);
    }

    {
        allocator_with_state<int> a;
        nestl::vector<int, allocator_with_state<int> > vec(a);

        CheckVectorSize(vec, 0);
    }

    {
        nestl::vector<int, allocator_with_state<int> > vec1;

		nestl::vector<int, allocator_with_state<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }

    {
        nestl::vector<int, allocator_with_state<int> > vec1;

        ASSERT_OPERATION_SUCCESS(vec1.assign_nothrow(_, 10));

		nestl::vector<int, allocator_with_state<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
