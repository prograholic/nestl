#ifndef NESTL_TESTS_NESTL_TEST_HPP
#define NESTL_TESTS_NESTL_TEST_HPP

#include "tests/nestl_printers.hpp"

#include <nestl/class_operations.hpp>

#include <tuple>
#include <iostream>


namespace nestl
{

namespace test
{

namespace detail
{


template<std::size_t...> struct seq
{
};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>
{
};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...>
{
};



template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os, const Tuple& t, seq<Is...>)
{
    using swallow = int[1 + sizeof...(Is)];
    (void)swallow
    {
        0,
        (void(os << /* (Is == 0 ? "" : ", ") << */ std::get<Is>(t)), 0)...
    };
}

template <typename ...Args>
void print_tuple(std::ostream& ostream, Args&&... args)
{
    print_tuple_impl(ostream, std::make_tuple(args...), gen_seq<sizeof...(Args)>());
}


} // namespace detail



template <typename ...Args>
void fatal_failure(Args&&... args)
{
    detail::print_tuple(std::cerr, std::forward<Args>(args)...);

    std::abort();
}


inline
void check_error(const nestl::default_operation_error& err, const char* msg)
{
    if (err)
    {
        fatal_failure("operation ", msg, " failed with following error: ", err);
    }
}

#define NESTL_CHECK_OPERATION(val) \
do \
{ \
 \
    nestl::default_operation_error _; \
    val; \
    check_error(_, #val); \
} while(0) \


#define NESTL_CHECK_EQ(left, right) \
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





typedef void(*TestRunner)();

const size_t MaxTestCount = 1000;

struct TestSuite
{
    TestSuite()
        : m_testRunners()
        , m_registeredTestRunners(0)
    {
    }

    void AddTest(TestRunner testRunner)
    {
        if (m_registeredTestRunners == MaxTestCount)
        {
            fatal_failure("test limit exceeeded");
        }
        else
        {
            m_testRunners[m_registeredTestRunners] = testRunner;
            m_registeredTestRunners += 1;
        }
    }

    void RunTests()
    {
        for (size_t i = 0; i != m_registeredTestRunners; ++i)
        {
            m_testRunners[i]();
        }
    }

    static TestSuite& GetInstance()
    {
        static TestSuite instance;
        return instance;
    }

private:
    TestRunner m_testRunners[MaxTestCount];
    size_t m_registeredTestRunners;
};



} // namespace test
} // namespace nestl

#define NESTL_PP_CAT2(x, y) x ## y
#define NESTL_PP_CAT(x, y) NESTL_PP_CAT2(x, y)


#define NESTL_ADD_TEST(test_name) \
template <size_t Unused> \
struct NESTL_PP_CAT(TestRegistrator ## test_name, __LINE__) \
{ \
    NESTL_PP_CAT(TestRegistrator ## test_name, __LINE__)() \
    { \
        TestSuite::GetInstance().AddTest(TestRunner); \
    } \
    static void TestRunner(); \
}; \
namespace {NESTL_PP_CAT(TestRegistrator ## test_name, __LINE__)<__COUNTER__> NESTL_PP_CAT(testRegistrator, __LINE__); }\
template <size_t Unused> \
void NESTL_PP_CAT(TestRegistrator ## test_name, __LINE__)<Unused>::TestRunner()\


#define NESTL_RUN_ALL_TESTS() nestl::test::TestSuite::GetInstance().RunTests()


#endif /* NESTL_TESTS_NESTL_TEST_HPP */
