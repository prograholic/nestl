#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(set_test_insert)
{

    {
        nestl::set<int> s;

        NESTL_CHECK_OPERATION(s.insert_nothrow(_, 10));

        CheckSetSize(s, 1);
    }
}

} // namespace test
} // namespace nestl
