#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{


class SetTestInsertBase : public SetTestBase
{
};

class SetTestInsertWithDefaultAllocator : public SetTestInsertBase
{
};


TEST_F(SetTestInsertWithDefaultAllocator, InsertOneElement)
{
    nestl::set<int> s;

    typename nestl::set<int>::operation_error error;
    s.insert_nothrow(error, 10);
    ASSERT_OPERATION_SUCCESS(error);

    EXPECT_TRUE(CheckSetSize(s, 1));
}

} // namespace test

} // namespace nestl
