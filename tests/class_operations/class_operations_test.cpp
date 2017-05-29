#include <nestl/class_operations.hpp>
#include <nestl/alignment.hpp>

#include "tests/test_common.hpp"


namespace nestl
{
namespace test
{

NESTL_ADD_TEST(class_operations_test)
{
    {
        nestl::aligned_buffer<int> x;
        int y = 2;

        NESTL_CHECK_OPERATION(class_operations::construct(_, x.ptr(), y));

        NESTL_CHECK_EQ(2, *x.ptr());

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        const non_copyable arg{42};

        static_assert(std::is_nothrow_constructible<non_copyable>::value, "WAT??");

        NESTL_CHECK_OPERATION(class_operations::construct(_, x.ptr(), arg));

        NESTL_CHECK_EQ(42, x.ptr()->v);

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        const non_copyable arg{ 42 };
        const int val = 10;

        NESTL_CHECK_OPERATION(class_operations::construct(_, x.ptr(), arg, val));

        NESTL_CHECK_EQ(52, x.ptr()->v);

        detail::destroy(x.ptr());
    }

    {
        nestl::aligned_buffer<non_copyable> x;
        non_copyable arg1{ 42 };
        non_copyable arg2{ 100500 };

        NESTL_CHECK_OPERATION(class_operations::construct(_, x.ptr(), arg1, std::move(arg2)));

        NESTL_CHECK_EQ(100542, x.ptr()->v);

        NESTL_CHECK_EQ(43, arg1.v);
        NESTL_CHECK_EQ(0, arg2.v);

        detail::destroy(x.ptr());
    }
}


} // namespace test
} // namespace nestl
