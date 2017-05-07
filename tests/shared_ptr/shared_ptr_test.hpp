#ifndef NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP
#define NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP

#include <nestl/shared_ptr.hpp>

#include "tests/test_common.hpp"

namespace nestl
{
namespace test
{

template <typename SharedPtr>
void CheckSharedPtr(const SharedPtr& sp, const long expectedRefCount)
{
    const bool shouldNotBeEmpty = (expectedRefCount != 0);
    const bool shouldBeUnique = (expectedRefCount == 1);

    bool operatorBoolResult = sp ? true : false;
    if (operatorBoolResult != shouldNotBeEmpty)
    {
        fatal_failure("explicit operator bool returns ", operatorBoolResult, ", expected ", shouldNotBeEmpty);
    }

    if (sp.use_count() != expectedRefCount)
    {
        fatal_failure("method use_count returns ", sp.use_count(), ", expected ", expectedRefCount);
    }

    if (sp.unique() != shouldBeUnique)
    {
        fatal_failure("method unique returns ", sp.unique(), ", expected ", shouldBeUnique);
    }

    bool isPointerNotZero = (sp.get() != 0);
    if (isPointerNotZero != shouldNotBeEmpty)
    {
        fatal_failure("method get returns ", (isPointerNotZero ? "not <NULL>" : "<NULL>"), ", expected ", (shouldNotBeEmpty ? "not <NULL>" : "<NULL>"));
    }
}


} // namespace test
} // namespace nestl


#endif /* NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP */
