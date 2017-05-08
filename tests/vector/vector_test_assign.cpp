#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{

void run_tests()
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
        ASSERT_OPERATION_SUCCESS(vec2.assign_nothrow(ec, 10));

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 0);
        CheckVectorSize(vec1, 0);
    }


    {
        nestl::vector<int> vec1;
        ASSERT_OPERATION_SUCCESS(vec1.assign_nothrow(ec, 10));

        nestl::vector<int> vec2;

		vec2 = std::move(vec1);
        CheckVectorSize(vec2, 10);
        CheckVectorSize(vec1, 0);
    }


    {
        nestl::vector<int> vec1;
        ASSERT_OPERATION_SUCCESS(vec1.assign_nothrow(ec, 10));

        nestl::vector<int> vec2;
        ASSERT_OPERATION_SUCCESS(vec2.assign_nothrow(ec, 5));

		vec2 = std::move(vec1);
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
