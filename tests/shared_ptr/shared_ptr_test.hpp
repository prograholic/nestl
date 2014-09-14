#ifndef NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP
#define NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP

#include <nestl/shared_ptr.hpp>

#include "tests/test_common.hpp"

namespace nestl
{

namespace test
{


/**
 * @brief Base class for all shared_ptr tests
 */
template <typename TypeWithAllocator>
class SharedPtrTestBase : public ::testing::Test
{
public:
    typedef typename TypeWithAllocator::value_type value_type;
    typedef typename TypeWithAllocator::allocator  allocator;

    typedef nestl::shared_ptr<value_type>          shared_ptr_t;
    typedef typename shared_ptr_t::operation_error operation_error;
};


/**
 * @brief Type list definitions for tests
 */

typedef ::testing::Types
<
    type_with_allocator<int>,
    type_with_allocator<non_copyable>
> SharedPtrCommonTypes;





template <typename SharedPtr>
testing::AssertionResult CheckSharedPtr(const SharedPtr& sp, const long expectedRefCount)
{
    const bool shouldNotBeEmpty = (expectedRefCount != 0);
    const bool shouldBeUnique = (expectedRefCount == 1);

    bool operatorBoolResult = sp ? true : false;
    if (operatorBoolResult != shouldNotBeEmpty)
    {
        return testing::AssertionFailure() <<
            "explicit operator bool returns " << operatorBoolResult << ", "
            "expected " << shouldNotBeEmpty;
    }

    if (sp.use_count() != expectedRefCount)
    {
        return testing::AssertionFailure() <<
            "method use_count returns " << sp.use_count() << ", "
            "expected " << expectedRefCount;
    }

    if (sp.unique() != shouldBeUnique)
    {
        return testing::AssertionFailure() <<
            "method unique returns " << sp.unique() << ", "
            "expected " << shouldBeUnique;
    }

    bool isPointerNotZero = (sp.get() != 0);
    if (isPointerNotZero != shouldNotBeEmpty)
    {
        return testing::AssertionFailure() <<
            "method get returns " << (isPointerNotZero ? "not <NULL>" : "<NULL>") << ", "
            "expected " << (shouldNotBeEmpty ? "not <NULL>" : "<NULL>");
    }

    return testing::AssertionSuccess();
}


} // namespace test

} // namespace nestl


#endif /* NESTL_TESTS_SHARED_PTR_SHARED_PTR_TEST_HPP */
