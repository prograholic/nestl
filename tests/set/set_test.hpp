#ifndef NESTL_TESTS_SET_SET_TEST_HPP
#define NESTL_TESTS_SET_SET_TEST_HPP

#include <nestl/set.hpp>

#include "tests/test_common.hpp"
#include <nestl/algorithm.hpp>

namespace nestl
{

namespace test
{

template <typename Set>
::testing::AssertionResult CheckSetSize(const Set& s, size_t expectedSize)
{
    if (s.size() != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "expected size: " << expectedSize << ", got: " << s.size();
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool setIsEmpty = s.empty();

    if (setIsEmpty != shouldBeEmpty)
    {
        return ::testing::AssertionFailure() <<
            "set empty method returns " << setIsEmpty << ", "
            "while it should be " << shouldBeEmpty;
    }

    size_t dist = nestl::distance(s.begin(), s.end());
    if (dist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between iterators should be " << expectedSize << " elements, "
            "got " << dist << " elements";
    }

    size_t cdist = nestl::distance(s.cbegin(), s.cend());
    if (cdist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between const iterators should be " << expectedSize << " elements, "
            "got " << cdist << " elements";
    }


    return ::testing::AssertionSuccess();
}


/**
 * @brief base class for all vector tests
 */
class SetTestBase : public ::testing::Test
{
};


} // namespace test

} // namespace nestl


#endif /* NESTL_TESTS_SET_SET_TEST_HPP */
