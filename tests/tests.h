#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <map>

#include <nestl/noncopyable.hpp>


#define DUMP_ASSERT_AND_EXIT(x, ...) \
    std::cerr << "assertion failed: " #x << "at " << __FILE__ << ":" << __LINE__ << " " << __VA_ARGS__ << std::endl; \
    std::exit(EXIT_FAILURE); \

// gtest-like macro
#define ASSERT_TRUE(x) do {if (!(x)) {DUMP_ASSERT_AND_EXIT(x, "")}} while (0)

#define ASSERT_SUCCESS(x) do {auto res = (x); if (res.value() != 0) {DUMP_ASSERT_AND_EXIT(x, ", returned result: " << res.message())}} while (0)


typedef std::map<const volatile void*, int> allocation_count_impl;

struct allocation_count
{
    allocation_count()
        : totalAllocationCount(0)
        , impl()
    {
    }

    size_t totalAllocationCount;
    allocation_count_impl impl;
};


inline void ensure_allocation_count_empty(allocation_count& acount)
{
    for (allocation_count_impl::const_iterator it = acount.impl.cbegin(); it != acount.impl.cend(); ++it)
    {
        ASSERT_TRUE(it->second == 0);
    }

    std::cerr << "total allocations: " << acount.totalAllocationCount << std::endl;

    /// reset
    acount = allocation_count();
}


struct NonCopyableButAssignCopyable : private nestl::noncopyable
{
    NonCopyableButAssignCopyable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 0);
        ++count;

        ++ms_count.totalAllocationCount;
    }

    ~NonCopyableButAssignCopyable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 1);
        --count;
    }

    std::error_condition assign_copy(const NonCopyableButAssignCopyable& other) noexcept
    {
        return std::error_condition();
    }

    static allocation_count ms_count;
};


struct NonCopyableButMoveable :  private nestl::noncopyable
{
    NonCopyableButMoveable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 0);
        ++count;

        ++ms_count.totalAllocationCount;
    }

    NonCopyableButMoveable(NonCopyableButMoveable&& other) noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 0);
    }

    ~NonCopyableButMoveable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 1);
        --count;
    }

    static allocation_count ms_count;
};



struct CopyableButNotMoveable
{
    CopyableButNotMoveable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 0);
        ++count;

        ++ms_count.totalAllocationCount;
    }

    CopyableButNotMoveable(const CopyableButNotMoveable& other) noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 0);
        ++count;

        ++ms_count.totalAllocationCount;
    }

    ~CopyableButNotMoveable() noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 1);
        --count;
    }

    CopyableButNotMoveable& operator =(const CopyableButNotMoveable& other) noexcept
    {
        int& count = ms_count.impl[this];
        NESTL_ASSERT(count == 1);

        return *this;
    }

    CopyableButNotMoveable(CopyableButNotMoveable&& other) = delete;

    static allocation_count ms_count;
};



struct TriviallyCopyable
{
    int val1;
    int val2;
};





void vector_test();


#endif // TESTS_TESTS_H
