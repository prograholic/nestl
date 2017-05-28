#include <nestl/class_operations.hpp>
#include <nestl/alignment.hpp>

#include "tests/test_common.hpp"


namespace nestl
{
namespace test
{

void run_tests()
{
    {
        nestl::aligned_buffer<int> x;
        int y = 2;

        ASSERT_OPERATION_SUCCESS(class_operations::construct(_, x.ptr(), y));

        EXPECT_EQ(2, *x.ptr());

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        const non_copyable arg{42};

        static_assert(std::is_nothrow_constructible<non_copyable>::value, "WAT??");

        ASSERT_OPERATION_SUCCESS(class_operations::construct(_, x.ptr(), arg));

        EXPECT_EQ(42, x.ptr()->v);

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        const non_copyable arg{ 42 };
        const int val = 10;

        ASSERT_OPERATION_SUCCESS(class_operations::construct(_, x.ptr(), arg, val));

        EXPECT_EQ(52, x.ptr()->v);

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        non_copyable arg1{ 42 };
        non_copyable arg2{ 100500 };

        ASSERT_OPERATION_SUCCESS(class_operations::construct(_, x.ptr(), arg1, std::move(arg2)));

        EXPECT_EQ(100542, x.ptr()->v);

        EXPECT_EQ(43, arg1.v);
        EXPECT_EQ(0, arg2.v);

        detail::destroy(x.ptr());
    }
}


} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
