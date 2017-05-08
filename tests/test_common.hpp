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



void check_error(const nestl::operation_error& err, const char* msg)
{
    if (err)
    {
        fatal_failure("operation ", msg, " failed with following error: ", err);
    }
}

#define ASSERT_OPERATION_SUCCESS(val) \
do \
{ \
 \
    nestl::operation_error _; \
    val; \
    check_error(_, #val); \
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

    void assign(nestl::operation_error& /* op */, const non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        v = other.v;
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
struct class_operations<test::non_copyable>
{
    template <typename Allocator>
    static void construct(typename Allocator::operation_error& err,
                          test::non_copyable* ptr,
                          Allocator& alloc,
                          const test::non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        allocator_traits<Allocator>::construct(err, ptr);
        if (err)
        {
            return;
        }

        test::non_copyable* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<test::non_copyable*, Allocator> guard(ptr, end, alloc);

        ptr->assign(err, other);
        if (err)
        {
            return;
        }

        guard.release();
    }
};


} // namespace nestl

#endif /* NESTL_TESTS_TEST_COMMON_HPP */
