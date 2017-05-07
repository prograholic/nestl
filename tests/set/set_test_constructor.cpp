#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{

void run_tests()
{

    {
        nestl::set<int> s;

        CheckSetSize(s, 0);
    }


    {
        nestl::allocator<int> a;
        nestl::set<int> s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int> s1;

        nestl::set<int> s2(nestl::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int> s1;

        ASSERT_OPERATION_SUCCESS(s1.insert(10));

        nestl::set<int> s2(nestl::move(s1));

        CheckSetSize(s2, 1);
        CheckSetSize(s1, 0);
    }



    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with zero allocator
    ////////////////////////////////////////////////////////////////////////////////
    {
        nestl::set<int, zero_allocator<int> > s;

        CheckSetSize(s, 0);
    }

    {
        zero_allocator<int> a;
        nestl::set<int, zero_allocator<int> > s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int, zero_allocator<int> > s1;

        nestl::set<int, zero_allocator<int> > s2(nestl::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with minimal allocator
    ////////////////////////////////////////////////////////////////////////////////
    {
        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s;

        CheckSetSize(s, 0);
    }

    {
        minimal_allocator<int> a;
        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s1;

        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s2(nestl::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s1;

        ASSERT_OPERATION_SUCCESS(s1.insert(10));

        nestl::set<int, nestl::less<int>, minimal_allocator<int> > s2(nestl::move(s1));

        CheckSetSize(s2, 1);
        CheckSetSize(s1, 0);
    }


    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with stateful allocator
    ////////////////////////////////////////////////////////////////////////////////

    {
        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s;

        CheckSetSize(s, 0);
    }

    {
        allocator_with_state<int> a;
        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s1;

        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s2(nestl::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s1;

        ASSERT_OPERATION_SUCCESS(s1.insert(10));

        nestl::set<int, nestl::less<int>, allocator_with_state<int> > s2(nestl::move(s1));

        CheckSetSize(s2, 1);
        CheckSetSize(s1, 0);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
