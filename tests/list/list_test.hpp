#ifndef NESTL_TESTS_LIST_LIST_TEST_HPP
#define NESTL_TESTS_LIST_LIST_TEST_HPP

#include <nestl/list.hpp>

#include "tests/test_common.hpp"

namespace nestl
{

namespace test
{

template <typename List>
void CheckListSize(const List& l, size_t expectedSize)
{
    if (l.size() != expectedSize)
    {
        fatal_failure("expected size: ", expectedSize, ", got: ", l.size());
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool listIsEmpty = l.empty();

    if (listIsEmpty != shouldBeEmpty)
    {
        fatal_failure("list empty method returns ", listIsEmpty, ", while it should be ", shouldBeEmpty);
    }

    size_t dist = std::distance(l.begin(), l.end());
    if (dist != expectedSize)
    {
        fatal_failure("distance between iterators should be ", expectedSize, " elements, got ", dist, " elements");
    }

    size_t cdist = std::distance(l.cbegin(), l.cend());
    if (cdist != expectedSize)
    {
        fatal_failure("distance between const iterators should be ", expectedSize, " elements, got ", cdist, " elements");
    }
}

} // namespace test
} // namespace nestl

#endif /* NESTL_TESTS_LIST_LIST_TEST_HPP */
