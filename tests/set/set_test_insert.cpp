#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{

void run_tests()
{

    {
        nestl::set<int> s;

        ASSERT_OPERATION_SUCCESS(s.insert_nothrow(_, 10));

        CheckSetSize(s, 1);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
