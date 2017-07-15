#ifndef NESTL_TESTS_VECTOR_VECTOR_TEST_HPP
#define NESTL_TESTS_VECTOR_VECTOR_TEST_HPP

#include <nestl/vector.hpp>

#include "tests/test_common.hpp"
#include "tests/allocators.hpp"

namespace nestl
{
namespace test
{


template <typename Vector>
void CheckVectorSize(const Vector& v, size_t expectedSize)
{
    if (v.size() != expectedSize)
    {
        fatal_failure("expected size: ", expectedSize, ", got: ", v.size());
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool vectorIsEmpty = v.empty();

    if (vectorIsEmpty != shouldBeEmpty)
    {
        fatal_failure("vector empty method returns ", vectorIsEmpty, ", while it should be ", shouldBeEmpty);
    }

    if (v.capacity() < v.size())
    {
        fatal_failure("capacity should be at least ", v.size(), " elements, got ", v.capacity(), " elements");
    }


    size_t dist = std::distance(v.begin(), v.end());
    if (dist != expectedSize)
    {
        fatal_failure("distance between iterators should be ", expectedSize, " elements, got ", dist, " elements");
    }

    size_t cdist = std::distance(v.cbegin(), v.cend());
    if (cdist != expectedSize)
    {
        fatal_failure("distance between const iterators should be ", expectedSize, " elements, got ", cdist, " elements");
    }
}


} // namespace test
} // namespace nestl


#endif /* NESTL_TESTS_VECTOR_VECTOR_TEST_HPP */
