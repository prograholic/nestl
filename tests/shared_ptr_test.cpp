#include "tests.h"


#include <nestl/shared_ptr.hpp>

#if 0

template <typename SharedPtr>
void ensure_shared_ptr(const SharedPtr& ptr, long use_count)
{
    ASSERT_TRUE(ptr.use_count() == use_count);
    ASSERT_TRUE(ptr.unique() == (use_count == 1));
    ASSERT_TRUE((ptr.get() == 0) == (use_count == 0));
    //ASSERT_TRUE((std::addressof(*ptr) == 0) == (use_count == 0));
    ASSERT_TRUE(((!!ptr) == false) == (use_count == 0));
}

template <typename WeakPtr>
void ensure_weak_ptr(const WeakPtr& ptr, long use_count)
{
    ASSERT_TRUE(ptr.use_count() == use_count);
    ASSERT_TRUE(ptr.expired() == (use_count == 0));
}


template <typename T, typename Y, typename SharedPtrConstructor>
void shared_ptr_common_test_ex()
{
    {
        nestl::shared_ptr<T> ptr;
        ensure_shared_ptr(ptr, 0);
    }
    {
        nestl::shared_ptr<T> ptr;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr));
        ensure_shared_ptr(ptr, SharedPtrConstructor::expected_use_count);
    }
    {
        nestl::shared_ptr<T> ptr1;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr1));

        nestl::shared_ptr<T> ptr2(ptr1);
        if (SharedPtrConstructor::expected_use_count)
        {
            ensure_shared_ptr(ptr1, SharedPtrConstructor::expected_use_count + 1);
            ensure_shared_ptr(ptr2, SharedPtrConstructor::expected_use_count + 1);
        }
        else
        {
            ensure_shared_ptr(ptr1, 0);
            ensure_shared_ptr(ptr2, 0);
        }
    }

    {
        nestl::shared_ptr<T> ptr1;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr1));

        nestl::shared_ptr<Y> ptr2(ptr1);
        if (SharedPtrConstructor::expected_use_count)
        {
            ensure_shared_ptr(ptr1, SharedPtrConstructor::expected_use_count + 1);
            ensure_shared_ptr(ptr2, SharedPtrConstructor::expected_use_count + 1);
        }
        else
        {
            ensure_shared_ptr(ptr1, 0);
            ensure_shared_ptr(ptr2, 0);
        }
    }

    {
        nestl::shared_ptr<T> ptr1;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr1));

        {
            nestl::shared_ptr<Y> ptr2(ptr1);
            if (SharedPtrConstructor::expected_use_count)
            {
                ensure_shared_ptr(ptr1, SharedPtrConstructor::expected_use_count + 1);
                ensure_shared_ptr(ptr2, SharedPtrConstructor::expected_use_count + 1);
            }
            else
            {
                ensure_shared_ptr(ptr1, 0);
                ensure_shared_ptr(ptr2, 0);
            }
        }
        ensure_shared_ptr(ptr1, SharedPtrConstructor::expected_use_count);
    }

    {
        nestl::shared_ptr<T> ptr1;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr1));

        nestl::shared_ptr<T> ptr2(std::move(ptr1));
        ensure_shared_ptr(ptr1, 0);
        ensure_shared_ptr(ptr2, SharedPtrConstructor::expected_use_count);
    }

    {
        nestl::shared_ptr<T> ptr;
        ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr));

        nestl::weak_ptr<T> wptr(ptr);
        ensure_shared_ptr(ptr, SharedPtrConstructor::expected_use_count);
        ensure_weak_ptr(wptr, SharedPtrConstructor::expected_use_count);

        nestl::shared_ptr<T> lockptr = wptr.lock();
        ensure_shared_ptr(ptr, SharedPtrConstructor::expected_use_count * 2);
        ensure_shared_ptr(lockptr, SharedPtrConstructor::expected_use_count * 2);

        ensure_weak_ptr(wptr, SharedPtrConstructor::expected_use_count * 2);
    }


    {
        nestl::weak_ptr<T> wptr;
        ensure_weak_ptr(wptr, 0);
        {
            nestl::shared_ptr<T> ptr;
            ASSERT_SUCCESS(SharedPtrConstructor::construct(ptr));
            ensure_shared_ptr(ptr, SharedPtrConstructor::expected_use_count);

            wptr = ptr;
            ensure_weak_ptr(wptr, SharedPtrConstructor::expected_use_count);


            nestl::shared_ptr<T> lockptr = wptr.lock();
            ensure_shared_ptr(ptr, SharedPtrConstructor::expected_use_count * 2);
            ensure_shared_ptr(lockptr, SharedPtrConstructor::expected_use_count * 2);

            ensure_weak_ptr(wptr, SharedPtrConstructor::expected_use_count * 2);
        }

        ensure_weak_ptr(wptr, 0);
    }
}

template <typename T, bool allocate>
struct shared_ptr_constructor
{
    enum {expected_use_count = allocate};

    typedef typename nestl::shared_ptr<T>::operation_error operation_error;

    static operation_error construct(nestl::shared_ptr<T>& ptr)
    {
        if (allocate)
        {
            nestl::allocator<T> alloc;
            operation_error err = nestl::make_shared_ex_a<T>(ptr, alloc);
            if (err)
            {
                return err;
            }
        }

        return operation_error();
    }
};


template <typename T>
void shared_ptr_common_test(const char* name)
{
    std::cerr << "test [" << name << "] started (shared_ptr_common_test)" << std::endl;

    shared_ptr_common_test_ex<T, T, shared_ptr_constructor<T, true>>();
    shared_ptr_common_test_ex<T, T, shared_ptr_constructor<T, false>>();


    std::cerr << "test [" << name << "] passed  (shared_ptr_common_test)" << std::endl;
}




void shared_ptr_test()
{
    shared_ptr_common_test<int>                         ("int                         ");

    shared_ptr_common_test<NonCopyableButAssignCopyable>("NonCopyableButAssignCopyable");
    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);

    shared_ptr_common_test<NonCopyableButMoveable>      ("NonCopyableButMoveable      ");
    ensure_allocation_count_empty(NonCopyableButMoveable::ms_count);

    shared_ptr_common_test<CopyableButNotMoveable>      ("CopyableButNotMoveable      ");
    ensure_allocation_count_empty(CopyableButNotMoveable::ms_count);

    shared_ptr_common_test<TriviallyCopyable>           ("TriviallyCopyable           ");
}


#endif //0
