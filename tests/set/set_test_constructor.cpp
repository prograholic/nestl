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

TEST_F(SetTestConstructorWithDefaultAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int> s1;

    nestl::set<int> s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}

TEST_F(SetTestConstructorWithDefaultAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::set<int> s1;

    typename nestl::set<int>::operation_error error;
    s1.insert_nothrow(error, 10);
    ASSERT_OPERATION_SUCCESS(error);

    nestl::set<int> s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 1));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}



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

TEST_F(SetTestConstructorWithZeroAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, zero_allocator<int> > s1;

    nestl::set<int, zero_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}



////////////////////////////////////////////////////////////////////////////////
/// Now check with minimal allocator
////////////////////////////////////////////////////////////////////////////////


class SetTestConstructorWithMinimalAllocator : public SetTestConstructorBase
{
};


TEST_F(SetTestConstructorWithMinimalAllocator, ConstructorWithoutParameters)
{
    nestl::set<int, nestl::less<int>, minimal_allocator<int> > s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithMinimalAllocator, ConstructorWithoutParametersWithAllocator)
{
    minimal_allocator<int> a;
    nestl::set<int, nestl::less<int>, minimal_allocator<int> > s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}

TEST_F(SetTestConstructorWithMinimalAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, nestl::less<int>, minimal_allocator<int> > s1;

    nestl::set<int, nestl::less<int>, minimal_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}

TEST_F(SetTestConstructorWithMinimalAllocator, MoveConstructorFromNonEmptyVector)
{
    typedef nestl::set<int, nestl::less<int>, minimal_allocator<int> > set_t;
    set_t s1;

    typename set_t::operation_error error;
    s1.insert_nothrow(error, 10);
    ASSERT_OPERATION_SUCCESS(error);

    nestl::set<int, nestl::less<int>, minimal_allocator<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 1));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}


////////////////////////////////////////////////////////////////////////////////
/// Now check with stateful allocator
////////////////////////////////////////////////////////////////////////////////

class SetTestConstructorWithStatefulAllocator : public SetTestConstructorBase
{
};

TEST_F(SetTestConstructorWithStatefulAllocator, ConstructorWithoutParameters)
{
    nestl::set<int, nestl::less<int>, allocator_with_state<int> > s;

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithStatefulAllocator, ConstructorWithoutParametersWithAllocator)
{
    allocator_with_state<int> a;
    nestl::set<int, nestl::less<int>, allocator_with_state<int> > s(a);

    EXPECT_TRUE(CheckSetSize(s, 0));
}


TEST_F(SetTestConstructorWithStatefulAllocator, MoveConstructorFromEmptyVector)
{
    nestl::set<int, nestl::less<int>, allocator_with_state<int> > s1;

    nestl::set<int, nestl::less<int>, allocator_with_state<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 0));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}


TEST_F(SetTestConstructorWithStatefulAllocator, MoveConstructorFromNonEmptyVector)
{
    typedef nestl::set<int, nestl::less<int>, allocator_with_state<int> > set_t;
    set_t s1;

    typename set_t::operation_error error;
    s1.insert_nothrow(error, 10);
    ASSERT_OPERATION_SUCCESS(error);

    nestl::set<int, nestl::less<int>, allocator_with_state<int> > s2(nestl::move(s1));

    EXPECT_TRUE(CheckSetSize(s2, 1));
    EXPECT_TRUE(CheckSetSize(s1, 0));
}

} // namespace test

} // namespace nestl
