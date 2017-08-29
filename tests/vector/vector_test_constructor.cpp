#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(vector_test_constructor)
{
    // check default ctor
    {
        nestl::vector<int> vec;

        CheckVectorSize(vec, 0);
    }

    // check ctor with allocator
    {
        nestl::allocator<int> a;
        nestl::vector<int> vec(a);

        CheckVectorSize(vec, 0);
    }

    // check move ctor (with empty source)
    {
        nestl::vector<int> vec1;

        nestl::vector<int> vec2(std::move(vec1));

        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }

    // check move ctor (with non-empty source)
    {
        nestl::vector<int> vec1;

        NESTL_CHECK_OPERATION(vec1.assign_nothrow(_, 10));

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

        NESTL_CHECK_OPERATION(vec1.assign_nothrow(_, 10));

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

        NESTL_CHECK_OPERATION(vec1.assign_nothrow(_, 10));

		nestl::vector<int, allocator_with_state<int> > vec2(std::move(vec1));

        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }

#if NESTL_HAS_EXCEPTIONS == 1
    ////////////////////////////////////////////////////////////////////////////////
    /// Now check throwable constructors
    ////////////////////////////////////////////////////////////////////////////////

    // check copy construction from empty vector
    {
        nestl::vector<int> v0;
        nestl::vector<int> v1{v0};

        CheckVectorSize(v1, 0);
    }

    // check copy construction from empty noexcept vector
    {
        nestl::no_exceptions::vector<int> nxv0;
        nestl::vector<int> v1{ nxv0 };

        CheckVectorSize(v1, 0);
    }

    // check copy construction from non-empty vector
    {
        nestl::vector<int> v0;
        v0.push_back(42);

        nestl::vector<int> v1{v0};

        CheckVectorSize(v0, 1);
        CheckVectorSize(v1, 1);

        NESTL_CHECK_EQ(42, v0[0]);
        NESTL_CHECK_EQ(42, v1[0]);
    }

    // check copy construction from non-empty noexcept vector
    {
        nestl::no_exceptions::vector<int> nxv0;
        NESTL_CHECK_OPERATION(nxv0.push_back_nothrow(_, 42));

        nestl::vector<int> v1{ nxv0 };

        CheckVectorSize(nxv0, 1);
        CheckVectorSize(v1, 1);

        NESTL_CHECK_EQ(42, nxv0[0]);
        NESTL_CHECK_EQ(42, v1[0]);
    }

    // check move construction from empty vector
    {
        nestl::vector<int> v0;
        nestl::vector<int> v1{ std::move(v0) };

        CheckVectorSize(v1, 0);
    }

    // check move construction from empty noexcept vector
    {
        nestl::no_exceptions::vector<int> nxv0;
        nestl::vector<int> v1{ std::move(nxv0) };

        CheckVectorSize(v1, 0);
    }

    // check move construction from non-empty vector
    {
        nestl::vector<int> v0;
        v0.push_back(42);

        nestl::vector<int> v1{ std::move(v0) };

        CheckVectorSize(v0, 0);
        CheckVectorSize(v1, 1);

        NESTL_CHECK_EQ(42, v1[0]);
    }

    // check move construction from non-empty noexcept vector
    {
        nestl::no_exceptions::vector<int> nxv0;
        NESTL_CHECK_OPERATION(nxv0.push_back_nothrow(_, 42));

        nestl::vector<int> v1{ std::move(nxv0) };

        CheckVectorSize(nxv0, 0);
        CheckVectorSize(v1, 1);

        NESTL_CHECK_EQ(42, v1[0]);
    }

#endif /* NESTL_HAS_EXCEPTIONS */
}

} // namespace test
} // namespace nestl
