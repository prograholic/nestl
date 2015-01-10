#ifndef NESTL_TESTS_GTEST_GMOCK_EMULATION_HPP
#define NESTL_TESTS_GTEST_GMOCK_EMULATION_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD

#   include <gtest/gtest.h>
#   include <gmock/gmock.h>

#else /* NESTL_USE_STD */

namespace testing
{

struct AssertionResult
{
    template <typename T>
    AssertionResult& operator << (const T&)
    {
        return *this;
    }
};




inline AssertionResult AssertionFailure()
{
    return AssertionResult();
}

inline AssertionResult AssertionSuccess()
{
    return AssertionResult();
}


struct Test
{
    virtual ~Test()
    {
    }
};


template <typename T0,
          typename T1 = void,
          typename T2 = void,
          typename T3 = void,
          typename T4 = void,
          typename T5 = void,
          typename T6 = void,
          typename T7 = void>
struct Types
{
};



struct FailureTracer
{
    template <typename T>
    FailureTracer& operator <<(const T& /* t */)
    {
        return *this;
    }
};


#define ADD_FAILURE() ::testing::FailureTracer()

#define EXPECT_TRUE(x)


#define TYPED_TEST_CASE_P(x) struct gtest_typed_test_case
#define REGISTER_TYPED_TEST_CASE_P(x, ...) struct gtest_typed_test_case_reg
#define INSTANTIATE_TYPED_TEST_CASE_P(x, ...) struct gtest_typed_test_case_inst

#define TYPED_TEST_P(fixture, name) \
    template <typename T> \
    class fixture ## name : public fixture<T> \
    { \
    public: \
        typedef fixture<T> TestFixture; \
        void TestBody(); \
    }; \
    template <typename T> void fixture ## name<T>::TestBody() \

#define TEST_F(fixture, name) \
    class fixture ## name \
    { \
    public: \
        void TestBody(); \
    }; \
    void fixture ## name::TestBody()\



#define ASSERT_PRED1(x, y)
#define EXPECT_PRED1(x, y)

} // namespace testing

#endif /* NESTL_USE_STD */

#endif /* NESTL_TESTS_GTEST_GMOCK_EMULATION_HPP */
