#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(vector_test_assign)
{
    ////////////////////////////////////////////////////////////////////////////
    /// Check move assignment
    ////////////////////////////////////////////////////////////////////////////

    // move assign empty vector to empty
    {
        nestl::vector<int> vec1;
        nestl::vector<int> vec2;

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }


    // self move assignment with non-empty vector
    {
        nestl::vector<int> vec;
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 10));

		vec = std::move(vec);
        CheckVectorSize(vec, 0);
    }

    // move assign non-empty vector to non-empty
    {
        nestl::vector<int> vec1;
        NESTL_CHECK_OPERATION(vec1.push_back_nothrow(_, 10));

        nestl::vector<int> vec2;
        NESTL_CHECK_OPERATION(vec2.push_back_nothrow(_, 5));

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 1);
        CheckVectorSize(vec1, 0);

        NESTL_CHECK_EQ(10, vec2[0]);
    }

#if NESTL_HAS_EXCEPTIONS == 1
    
    ////////////////////////////////////////////////////////////////////////////
    /// Check move assignment for throwable vector
    ////////////////////////////////////////////////////////////////////////////

    // move assign empty vector to empty
    {
        nestl::no_exceptions::vector<int> nxvec1;
        nestl::has_exceptions::vector<int> vec2;

        vec2 = std::move(nxvec1);
        CheckVectorSize(vec2, 0);
        CheckVectorSize(nxvec1, 0);
    }


    // self move assignment
    {
        nestl::vector<int> vec;
        vec.push_back(10);

        vec = std::move(vec.as_noexcept());
        CheckVectorSize(vec, 0);
    }

    // move assign non-empty vector to non-empty
    {
        nestl::no_exceptions::vector<int> nxvec1;
        NESTL_CHECK_OPERATION(nxvec1.push_back_nothrow(_, 10));

        nestl::has_exceptions::vector<int> vec2;
        vec2.push_back(5);

        vec2 = std::move(nxvec1);
        CheckVectorSize(vec2, 1);
        CheckVectorSize(nxvec1, 0);

        NESTL_CHECK_EQ(10, vec2[0]);
    }



    ////////////////////////////////////////////////////////////////////////////
    /// Check copy assignment for throwable vector
    ////////////////////////////////////////////////////////////////////////////

    // assign empty vector to empty
    {
        nestl::no_exceptions::vector<int> nxvec1;
        nestl::has_exceptions::vector<int> vec2;

        vec2 = nxvec1;
        CheckVectorSize(vec2, 0);
        CheckVectorSize(nxvec1, 0);
    }


    // self assignment
    {
        nestl::vector<int> vec;
        vec.push_back(10);

        vec = vec;
        CheckVectorSize(vec, 1);

        NESTL_CHECK_EQ(10, vec[0]);
    }

    // self assignment from noexcept
    {
        nestl::vector<int> vec;
        vec.push_back(10);

        vec = vec.as_noexcept();
        CheckVectorSize(vec, 1);

        NESTL_CHECK_EQ(10, vec[0]);
    }

    // assign non-empty vector to non-empty
    {
        nestl::no_exceptions::vector<int> nxvec1;
        NESTL_CHECK_OPERATION(nxvec1.push_back_nothrow(_, 10));

        nestl::has_exceptions::vector<int> vec2;
        vec2.push_back(5);

        vec2 = nxvec1;
        CheckVectorSize(vec2, 1);
        CheckVectorSize(nxvec1, 1);

        NESTL_CHECK_EQ(10, vec2[0]);
        NESTL_CHECK_EQ(10, nxvec1[0]);
    }

#endif /* NESTL_HAS_EXCEPTIONS */

}

} // namespace test
} // namespace nestl
