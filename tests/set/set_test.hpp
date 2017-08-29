#ifndef NESTL_TESTS_SET_SET_TEST_HPP
#define NESTL_TESTS_SET_SET_TEST_HPP

#include <nestl/set.hpp>

#include "tests/test_common.hpp"
#include "tests/allocators.hpp"

#include <nestl/algorithm.hpp>

namespace nestl
{

namespace test
{

template <typename Set>
void CheckSetSize(const Set& s, size_t expectedSize)
{
    if (s.size() != expectedSize)
    {
        fatal_failure("expected size: ", expectedSize, ", got: ", s.size());
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool setIsEmpty = s.empty();

    if (setIsEmpty != shouldBeEmpty)
    {
        fatal_failure("set empty method returns ", setIsEmpty, ", while it should be ", shouldBeEmpty);
    }

    size_t dist = std::distance(s.begin(), s.end());
    if (dist != expectedSize)
    {
        fatal_failure("distance between iterators should be ", expectedSize, " elements, got ", dist, " elements");
    }

    size_t cdist = std::distance(s.cbegin(), s.cend());
    if (cdist != expectedSize)
    {
        fatal_failure("distance between const iterators should be " ,expectedSize, " elements, got ", cdist, " elements");
    }
}


} // namespace test

} // namespace nestl


#endif /* NESTL_TESTS_SET_SET_TEST_HPP */
