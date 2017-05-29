#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(vector_test_assign)
{
    {
        nestl::vector<int> vec1;
        nestl::vector<int> vec2;

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }


    {
        nestl::vector<int> vec1;
        nestl::vector<int> vec2;
        NESTL_CHECK_OPERATION(vec2.assign_nothrow(_, 10));

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }


    {
        nestl::vector<int> vec1;
        NESTL_CHECK_OPERATION(vec1.assign_nothrow(_, 10));

        nestl::vector<int> vec2;

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }


    {
        nestl::vector<int> vec1;
        NESTL_CHECK_OPERATION(vec1.assign_nothrow(_, 10));

        nestl::vector<int> vec2;
        NESTL_CHECK_OPERATION(vec2.assign_nothrow(_, 5));

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }
}

} // namespace test
} // namespace nestl
