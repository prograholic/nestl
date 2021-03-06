#ifndef NESTL_TESTS_TEST_COMMON_HPP
#define NESTL_TESTS_TEST_COMMON_HPP

#include <nestl/class_operations.hpp>

#include "tests/nestl_test.hpp"

namespace nestl
{
namespace test
{


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

    non_copyable(non_copyable&& other) NESTL_NOEXCEPT_SPEC
        : v(other.v)
    {
        other.v = 0;
    }
    non_copyable& operator=(non_copyable&& other) NESTL_NOEXCEPT_SPEC
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

inline
bool operator == (const non_copyable& left, const non_copyable& right)
{
    return left.v == right.v;
}

inline
std::ostream& operator << (std::ostream& strm, const non_copyable& val)
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
