#ifndef NESTL_TESTS_TEST_COMMON_HPP
#define NESTL_TESTS_TEST_COMMON_HPP

#include "tests/nestl_printers.hpp"
#include "tests/allocators.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace nestl
{

namespace test
{

struct checker
{
    ::testing::AssertionResult operator()(const nestl::error_condition& ec)
    {
        if (ec)
        {
            return ::testing::AssertionFailure() <<
                "operation failed with following error: " << ec;
        }

        return ::testing::AssertionSuccess();
    }

    template <typename Result, typename OperationError>
    ::testing::AssertionResult operator()(const nestl::result_with_operation_error<Result, OperationError>& ec)
    {
        if (ec)
        {
            return ::testing::AssertionFailure() <<
                "operation failed with following error: " << ec.error();
        }

        return ::testing::AssertionSuccess();
    }
};


#define EXPECT_OPERATION_SUCCESS(val) EXPECT_PRED1(checker(), val)
#define ASSERT_OPERATION_SUCCESS(val) ASSERT_PRED1(checker(), val)



/**
 * @brief Special type which wraps type and corresponding allocator
 */
template <typename T, typename Allocator = nestl::allocator<T> >
struct type_with_allocator
{
    typedef T         value_type;
    typedef Allocator allocator;
};




struct non_copyable : private nestl::noncopyable
{
    non_copyable() NESTL_NOEXCEPT_SPEC
        : v(0)
    {
    }

    non_copyable(int x) NESTL_NOEXCEPT_SPEC
        : v(x)
    {
    }

    nestl::error_condition assign(const non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        v = other.v;
        return nestl::error_condition();
    }

    int v;
};

bool operator == (const non_copyable& left, const non_copyable& right)
{
    return left.v == right.v;
}

inline std::ostream& operator << (std::ostream& strm, const non_copyable& val)
{
    strm << "non_copyable { v : " << val.v << " };";
    return strm;
}

} // namespace test



template <>
struct class_traits<test::non_copyable>
{
    template <typename OperationError, typename Allocator>
    static OperationError construct(test::non_copyable* ptr,
                                    Allocator& alloc,
                                    const test::non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);

        test::non_copyable* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<test::non_copyable*, Allocator> guard(ptr, end, alloc);

        OperationError err = ptr->assign(other);
        if (err)
        {
            return err;
        }

        guard.release();
        return err;
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(test::non_copyable* ptr, Allocator& alloc) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);
        return OperationError();
    }
};


} // namespace nestl

#endif /* NESTL_TESTS_TEST_COMMON_HPP */
