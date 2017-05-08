#ifndef NESTL_TESTS_TEST_COMMON_HPP
#define NESTL_TESTS_TEST_COMMON_HPP

#include "tests/nestl_printers.hpp"

#include <tuple>
#include <iostream>


namespace nestl
{

namespace test
{

template <typename ...Args>
void print_tuple(std::ostream& ostream, Args&&... args)
{
    ostream << "not implemented";
    ostream << std::endl;
}

template <typename ...Args>
void fatal_failure(Args&&... args)
{
    print_tuple(std::cerr, std::forward<Args>(args)...);

    std::abort();
}



void check_ec(const nestl::error_condition& ec, const char* msg)
{
    if (ec)
    {
        fatal_failure("operation ", msg, " failed with following error: ", ec);
    }
}

#define ASSERT_OPERATION_SUCCESS(val) \
do \
{ \
 \
    error_condition ec; \
    val; \
    check_ec(ec, #val); \
} while(0) \


#define EXPECT_EQ(left, right) \
do \
{ \
 \
    auto left_c = (left); \
    auto right_c = (right); \
    if (left_c != right_c) \
    { \
        fatal_failure("expected " #left " == " #right ", left:", left_c, ", right: ", right_c); \
    } \
} while(0) \

/**
 * @brief Special type which wraps type and corresponding allocator
 */
template <typename T, typename Allocator = nestl::allocator<T> >
struct type_with_allocator
{
    typedef T         value_type;
    typedef Allocator allocator;
};




struct non_copyable
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

    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
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
