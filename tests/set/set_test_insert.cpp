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

    s.insert(10);

    EXPECT_TRUE(CheckSetSize(s, 1));
}

} // namespace test

} // namespace nestl
