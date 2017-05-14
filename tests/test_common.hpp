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



void check_error(const nestl::default_operation_error& err, const char* msg)
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
    nestl::default_operation_error _; \
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

    non_copyable(non_copyable&& other)
        : v(other.v)
    {
        other.v = 0;
    }
    non_copyable& operator=(non_copyable&& other)
    {
        if (this != &other)
        {
            v = other.v;
            other.v = 0;
        }

        return *this;
    }

    template <typename OperationError>
    void assign(OperationError& /* err */, const non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        v = other.v;
    }

    template <typename OperationError>
    void assign(OperationError& /* err */, non_copyable& other1, non_copyable&& other2) NESTL_NOEXCEPT_SPEC
    {
        *this = std::move(other2);
        v += other1.v;

        other1.v += 1;
    }

    int v;

private:
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
struct two_phase_initializator<test::non_copyable>
{
    template <typename OperationError>
    static void init(OperationError& err,
                     test::non_copyable& defaultConstructed,
                     const test::non_copyable& other) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.assign(err, other);
    }


    template <typename OperationError>
    static void init(OperationError& err,
                     test::non_copyable& defaultConstructed,
                     const test::non_copyable& other, int val) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.assign(err, other.v + val);
    }

    template <typename OperationError>
    static void init(OperationError& err,
                     test::non_copyable& defaultConstructed,
                     test::non_copyable& other1, test::non_copyable&& other2) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.assign(err, other1, std::move(other2));
    }
};


} // namespace nestl

#endif /* NESTL_TESTS_TEST_COMMON_HPP */
