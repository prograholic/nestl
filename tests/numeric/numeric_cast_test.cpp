#include <nestl/numeric/casting.hpp>

#include "tests/test_common.hpp"


class NumericTest : public ::testing::Test
{
};
using namespace nestl;
using namespace nestl::numeric;

TEST_F(NumericTest, CheckCastStatus)
{
    int val = -10;
    EXPECT_EQ(cast_rejected_negative_overflow, cast_status<unsigned short>(val));
}
