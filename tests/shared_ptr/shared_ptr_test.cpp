#include "tests/shared_ptr/shared_ptr_test.hpp"


namespace nestl
{
namespace test
{


template <typename TypeWithAllocator>
class SharedPtrTestConstructor : public SharedPtrTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(SharedPtrTestConstructor);


TYPED_TEST_P(SharedPtrTestConstructor, ConstructEmpty)
{
    typename TestFixture::shared_ptr_t sp;

    EXPECT_TRUE(CheckSharedPtr(sp, 0));
}


REGISTER_TYPED_TEST_CASE_P(SharedPtrTestConstructor,
                           ConstructEmpty);

INSTANTIATE_TYPED_TEST_CASE_P(shared_ptr_constructor_test, SharedPtrTestConstructor, SharedPtrCommonTypes);


////////////////////////////////////////////////////////////////////////////////


template <typename TypeWithAllocator>
class SharedPtrTestMakeShared : public SharedPtrTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(SharedPtrTestMakeShared);


TYPED_TEST_P(SharedPtrTestMakeShared, SimpleMakeShared)
{
    typename TestFixture::shared_ptr_t sp;

    ASSERT_OPERATION_SUCCESS(make_shared(sp));
    EXPECT_TRUE(CheckSharedPtr(sp, 1));
}


REGISTER_TYPED_TEST_CASE_P(SharedPtrTestMakeShared,
                           SimpleMakeShared);

INSTANTIATE_TYPED_TEST_CASE_P(shared_ptr_make_shared_test, SharedPtrTestMakeShared, SharedPtrCommonTypes);


} // namespace test

} // namespace nestl
