#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <map>

#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>


#define DUMP_ASSERT_AND_EXIT(x, ...) \
    std::cerr << "assertion failed: " #x << " at " << __FILE__ << ":" << __LINE__ << " " << __VA_ARGS__ << std::endl; \
    std::exit(EXIT_FAILURE); \

// gtest-like macro
#define ASSERT_TRUE(x) do {if (!(x)) {DUMP_ASSERT_AND_EXIT(x, "")}} while (0)


#define ASSERT_SUCCESS_EX(x, msg) do {auto res = (x); if (res.value() != 0) {DUMP_ASSERT_AND_EXIT(x, msg)}} while (0)

#define ASSERT_SUCCESS(x) ASSERT_SUCCESS_EX(x, ", returned result: " << res.message())


#define ASSERT_FAILURE_EX(x, msg) do {auto res = (x); if (res.value() == 0) {DUMP_ASSERT_AND_EXIT(x, msg)}} while (0)

#define ASSERT_FAILURE(x) ASSERT_FAILURE_EX(x, "expected failure")

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
        if (return_error_on_assign)
        {
            return std::error_condition(std::errc::not_enough_memory);
        }

        return std::error_condition();
    }

    static allocation_count ms_count;

    static bool return_error_on_assign;
};


namespace nestl
{

template <>
struct class_traits<NonCopyableButAssignCopyable>
{
    template<typename OperationError, typename Allocator>
    static OperationError construct(NonCopyableButAssignCopyable* ptr, Allocator& alloc) noexcept
    {
        alloc.construct(ptr);

        return OperationError();
    }

    template<typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(NonCopyableButAssignCopyable* ptr, Allocator& alloc, Args&& ... args) noexcept
    {
        alloc.construct(ptr);

        NonCopyableButAssignCopyable* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<NonCopyableButAssignCopyable*, Allocator> guard(ptr, end, alloc);

        OperationError err = ptr->assign_copy(std::forward<Args>(args) ...);
        if (err)
        {
            return err;
        }

        guard.release();

        return err;
    }

    template <typename OperationError, typename Y>
    static OperationError assign(NonCopyableButAssignCopyable& dest, Y&& src) noexcept
    {
        return dest.assign_copy(src);
    }
};

}


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


namespace nestl
{

template <>
struct class_traits<NonCopyableButMoveable>
{
    template<typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(NonCopyableButMoveable* ptr, Allocator& alloc, Args&& ... args) noexcept
    {
        alloc.construct(ptr, std::move<Args>(args) ...);

        return OperationError();
    }
};

}


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
void shared_ptr_test();
void algorithm_test();

#endif // TESTS_TESTS_H
