#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{


class SetTestConstructorBase : public SetTestBase
{
};

class SetTestConstructorWithDefaultAllocator : public SetTestConstructorBase
{
};


TEST_F(SetTestConstructorWithDefaultAllocator, ConstructorWithoutParameters)
{
    nestl::set<int> s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithDefaultAllocator, ConstructorWithoutParametersWithAllocator)
{
    nestl::allocator<int> a;
    nestl::set<int> s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}

#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithDefaultAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int> s1;

    nestl::set<int> s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */


#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithDefaultAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::set<int> s1;

    ASSERT_OPERATION_SUCCESS(s1.assign(10));

    nestl::set<int> s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(vec2, 10));
    EXPECT_TRUE(CheckSetSize(vec1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */




////////////////////////////////////////////////////////////////////////////////
/// Now check with zero allocator
////////////////////////////////////////////////////////////////////////////////


class SetTestConstructorWithZeroAllocator : public SetTestConstructorBase
{
};


TEST_F(SetTestConstructorWithZeroAllocator, ConstructorWithoutParameters)
{
    nestl::set<int, zero_allocator<int> > s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithZeroAllocator, ConstructorWithoutParametersWithAllocator)
{
    zero_allocator<int> a;
    nestl::set<int, zero_allocator<int> > s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}

#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithZeroAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, zero_allocator<int> > s1;

    nestl::set<int, zero_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */




////////////////////////////////////////////////////////////////////////////////
/// Now check with minimal allocator
////////////////////////////////////////////////////////////////////////////////


class SetTestConstructorWithMinimalAllocator : public SetTestConstructorBase
{
};


TEST_F(SetTestConstructorWithMinimalAllocator, ConstructorWithoutParameters)
{
    nestl::set<int, minimal_allocator<int> > s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithMinimalAllocator, ConstructorWithoutParametersWithAllocator)
{
    minimal_allocator<int> a;
    nestl::set<int, minimal_allocator<int> > s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}

#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithMinimalAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, minimal_allocator<int> > s1;

    nestl::set<int, minimal_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */

#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithMinimalAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::set<int, minimal_allocator<int> > s1;

    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::set<int, minimal_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 10));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */


////////////////////////////////////////////////////////////////////////////////
/// Now check with stateful allocator
////////////////////////////////////////////////////////////////////////////////

#if NESTL_TEST_HAS_ALLOCATOR_WITH_STATE

class SetTestConstructorWithStatefulAllocator : public SetTestConstructorBase
{
};

TEST_F(SetTestConstructorWithStatefulAllocator, ConstructorWithoutParameters)
{
    nestl::set<int, allocator_with_state<int> > s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithStatefulAllocator, ConstructorWithoutParametersWithAllocator)
{
    allocator_with_state<int> a;
    nestl::set<int, allocator_with_state<int> > s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}


#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithStatefulAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, allocator_with_state<int> > s1;

    nestl::set<int, allocator_with_state<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */

#if NESTL_HAS_RVALUE_REF
TEST_F(SetTestConstructorWithStatefulAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::set<int, allocator_with_state<int> > s1;

    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::set<int, allocator_with_state<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 10));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}
#endif /* NESTL_HAS_RVALUE_REF */

#endif /* #if NESTL_TEST_HAS_ALLOCATOR_WITH_STATE */

} // namespace test

} // namespace nestl
