#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(set_test_constructor)
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

        nestl::set<int> s2(std::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int> s1;

        NESTL_CHECK_OPERATION(s1.insert_nothrow(_, 10));

        nestl::set<int> s2(std::move(s1));

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

        nestl::set<int, zero_allocator<int> > s2(std::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with minimal allocator
    ////////////////////////////////////////////////////////////////////////////////
    {
        nestl::set<int, std::less<int>, minimal_allocator<int> > s;

        CheckSetSize(s, 0);
    }

    {
        minimal_allocator<int> a;
        nestl::set<int, std::less<int>, minimal_allocator<int> > s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int, std::less<int>, minimal_allocator<int> > s1;

        nestl::set<int, std::less<int>, minimal_allocator<int> > s2(std::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int, std::less<int>, minimal_allocator<int> > s1;

        NESTL_CHECK_OPERATION(s1.insert_nothrow(_, 10));

        nestl::set<int, std::less<int>, minimal_allocator<int> > s2(std::move(s1));

        CheckSetSize(s2, 1);
        CheckSetSize(s1, 0);
    }


    ////////////////////////////////////////////////////////////////////////////////
    /// Now check with stateful allocator
    ////////////////////////////////////////////////////////////////////////////////

    {
        nestl::set<int, std::less<int>, allocator_with_state<int> > s;

        CheckSetSize(s, 0);
    }

    {
        allocator_with_state<int> a;
        nestl::set<int, std::less<int>, allocator_with_state<int> > s(a);

        CheckSetSize(s, 0);
    }

    {
        nestl::set<int, std::less<int>, allocator_with_state<int> > s1;

        nestl::set<int, std::less<int>, allocator_with_state<int> > s2(std::move(s1));

        CheckSetSize(s2, 0);
        CheckSetSize(s1, 0);
    }

    {
        nestl::set<int, std::less<int>, allocator_with_state<int> > s1;

        NESTL_CHECK_OPERATION(s1.insert_nothrow(_, 10));

        nestl::set<int, std::less<int>, allocator_with_state<int> > s2(std::move(s1));

        CheckSetSize(s2, 1);
        CheckSetSize(s1, 0);
    }
}

} // namespace test
} // namespace nestl
