#include "tests.h"

#include <nestl/vector.hpp>


namespace
{


template <typename Vector>
void ensure_vector_size(Vector& vec, size_t expectedSize)
{
    ASSERT_TRUE(vec.size() == expectedSize);
    ASSERT_TRUE(vec.capacity() >= expectedSize);

    if (expectedSize == 0)
    {
        ASSERT_TRUE(vec.empty());
        ASSERT_TRUE(vec.begin() == vec.end());
        ASSERT_TRUE(vec.cbegin() == vec.cend());
        ASSERT_TRUE(vec.rbegin() == vec.rend());
        ASSERT_TRUE(vec.crbegin() == vec.crend());
    }
    else
    {
        ASSERT_TRUE(!vec.empty());
    }

    ASSERT_TRUE(expectedSize == std::distance(vec.begin(), vec.end()));
    ASSERT_TRUE(expectedSize == std::distance(vec.cbegin(), vec.cend()));
    ASSERT_TRUE(expectedSize == std::distance(vec.rbegin(), vec.rend()));
    ASSERT_TRUE(expectedSize == std::distance(vec.crbegin(), vec.crend()));
}

template <typename Vector, typename VectorConstructor>
void test_vector_assignment_ex()
{
    typedef typename Vector::value_type value_type;

    {
        /// assign empty vector to another
        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;

        ASSERT_SUCCESS(v1.assign_copy(v2));

        ensure_vector_size(v1, 0);
        ensure_vector_size(v2, 0);
    }

    {
        /// assign vector to empty
        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;

        ASSERT_SUCCESS(v2.assign_copy(v1));

        ensure_vector_size(v1, VectorConstructor::expected_size);
        ensure_vector_size(v2, VectorConstructor::expected_size);
    }

    {
        /// assign one element to vector
        Vector v;
        ASSERT_SUCCESS(VectorConstructor::construct(v));

        ASSERT_SUCCESS(v.assign(1));

        ensure_vector_size(v, 1);
    }

    {
        /// assign one vector to another
        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;
        ASSERT_SUCCESS(VectorConstructor::construct(v2));

        ASSERT_SUCCESS(v1.assign_copy(v2));

        ensure_vector_size(v1, VectorConstructor::expected_size);
        ensure_vector_size(v2, VectorConstructor::expected_size);
    }

    {
        Vector v;
        ASSERT_SUCCESS(VectorConstructor::construct(v));

        ASSERT_SUCCESS(v.emplace_back());
        ensure_vector_size(v, VectorConstructor::expected_size + 1);
    }
}



template <typename Vector, typename VectorConstructor>
void test_common_vector_operations_ex()
{
    {
        /// construct vector from other vector (move)
        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));
        Vector v2(std::move(v1));
        ensure_vector_size(v2, VectorConstructor::expected_size);
    }

    {
        /// request vector's allocator
        Vector v;
        ASSERT_SUCCESS(VectorConstructor::construct(v));
        typename Vector::allocator_type alloc = v.get_allocator();
        ensure_vector_size(v, VectorConstructor::expected_size);
    }

    {
        /// assign vector (move)
        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;

        v2 = std::move(v1);
        ensure_vector_size(v2, VectorConstructor::expected_size);
    }

    {
        /// swap vector with empty vector

        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;

        v2.swap(v1);

        ensure_vector_size(v2, VectorConstructor::expected_size);
        ensure_vector_size(v1, 0);
    }

    {
        /// swap vector with another vector

        Vector v1;
        ASSERT_SUCCESS(VectorConstructor::construct(v1));

        Vector v2;
        ASSERT_SUCCESS(VectorConstructor::construct(v2));

        v2.swap(v1);

        ensure_vector_size(v2, VectorConstructor::expected_size);
        ensure_vector_size(v1, VectorConstructor::expected_size);
    }

    {
        /// clear empty vector

        Vector v;
        v.clear();
        ensure_vector_size(v, 0);
    }

    {
        /// clear vector

        Vector v;
        ASSERT_SUCCESS(VectorConstructor::construct(v));

        v.clear();
        ensure_vector_size(v, 0);
    }

    {
        /// shrink_to_fit empty vector

        Vector v;
        v.shrink_to_fit();
        ensure_vector_size(v, 0);
    }

    {
        /// shrink_to_fit vector

        Vector v;
        ASSERT_SUCCESS(VectorConstructor::construct(v));

        v.shrink_to_fit();
        ensure_vector_size(v, VectorConstructor::expected_size);
        ASSERT_TRUE(v.capacity() == VectorConstructor::expected_size);
    }
}


template<size_t count>
struct resize_vector_constructor
{
    enum
    {
        expected_size = count
    };

    template <typename Vector>
    static std::error_condition construct(Vector& v)
    {
        ensure_vector_size(v, 0);

        return v.resize(count);
    }
};


template <typename T, typename A>
void test_common_vector_operations(const char* name)
{
    std::cerr << "test [" << name << "] started (test_common_vector_operations)" << std::endl;
    typedef nestl::vector<T, A> vector_t;

    {
        /// construct and destruct empty vector
        vector_t v;
        ensure_vector_size(v, 0);
    }

    test_common_vector_operations_ex<vector_t, resize_vector_constructor<0>>();
    test_common_vector_operations_ex<vector_t, resize_vector_constructor<1>>();
    test_common_vector_operations_ex<vector_t, resize_vector_constructor<2>>();
    test_common_vector_operations_ex<vector_t, resize_vector_constructor<100>>();

    std::cerr << "test [" << name << "] passed  (test_common_vector_operations)" << std::endl;
}


template <typename T, typename A>
void test_vector_assignment(const char* name)
{
    std::cerr << "test [" << name << "] started (test_vector_assignment)" << std::endl;
    typedef nestl::vector<T, A> vector_t;

    {
        /// construct and destruct empty vector
        vector_t v;
        ensure_vector_size(v, 0);
    }

    test_vector_assignment_ex<vector_t, resize_vector_constructor<0>>();
    test_vector_assignment_ex<vector_t, resize_vector_constructor<1>>();
    test_vector_assignment_ex<vector_t, resize_vector_constructor<2>>();
    test_vector_assignment_ex<vector_t, resize_vector_constructor<100>>();

    std::cerr << "test [" << name << "] passed  (test_vector_assignment)" << std::endl;
}



void common_test()
{
    test_common_vector_operations<int, nestl::allocator<int>>(
                "int,                                nestl::allocator<int>                         ");

    test_common_vector_operations<NonCopyableButAssignCopyable, nestl::allocator<NonCopyableButAssignCopyable>>(
                "NonCopyableButAssignCopyable,       nestl::allocator<NonCopyableButAssignCopyable>");
    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);

    test_common_vector_operations<NonCopyableButMoveable, nestl::allocator<NonCopyableButMoveable>>(
                "NonCopyableButMoveable,             nestl::allocator<NonCopyableButMoveable>      ");
    ensure_allocation_count_empty(NonCopyableButMoveable::ms_count);

    test_common_vector_operations<CopyableButNotMoveable, nestl::allocator<CopyableButNotMoveable>>(
                "CopyableButNotMoveable,             nestl::allocator<CopyableButNotMoveable>      ");
    ensure_allocation_count_empty(CopyableButNotMoveable::ms_count);

    test_common_vector_operations<TriviallyCopyable, nestl::allocator<TriviallyCopyable>>(
                "TriviallyCopyable,                  nestl::allocator<TriviallyCopyable>           ");


    ///////////////////////////////////////////////////////////////////////////////////////////


    test_vector_assignment<int, nestl::allocator<int>>(
                "int,                                nestl::allocator<int>                         ");

    test_vector_assignment<NonCopyableButAssignCopyable, nestl::allocator<NonCopyableButAssignCopyable>>(
                "NonCopyableButAssignCopyable,       nestl::allocator<NonCopyableButAssignCopyable>");
    ensure_allocation_count_empty(NonCopyableButAssignCopyable::ms_count);

    test_vector_assignment<CopyableButNotMoveable, nestl::allocator<CopyableButNotMoveable>>(
                "CopyableButNotMoveable,             nestl::allocator<CopyableButNotMoveable>      ");
    ensure_allocation_count_empty(CopyableButNotMoveable::ms_count);

    test_vector_assignment<TriviallyCopyable, nestl::allocator<TriviallyCopyable>>(
                "TriviallyCopyable,                  nestl::allocator<TriviallyCopyable>           ");
}




void test_vector_int()
{
    std::cerr << "test of vector<int> started" << std::endl;
    typedef nestl::vector<int> int_vector_t;

    int_vector_t v;


    ASSERT_SUCCESS(v.assign({0, 1, 2}));
    ensure_vector_size(v, 3);

    int val = 0;
    for (int_vector_t::const_iterator i = v.cbegin(); i != v.cend(); ++i, ++val)
    {
        ASSERT_TRUE(*i == val);
    }

    val = 2;
    for (int_vector_t::const_reverse_iterator i = v.crbegin(); i != v.crend(); ++i, --val)
    {
        ASSERT_TRUE(*i == val);
    }


    ASSERT_SUCCESS(v.insert(v.begin(), {-2, -1}));
    ensure_vector_size(v, 5);

    val = -2;
    for (int_vector_t::const_iterator i = v.cbegin(); i != v.cend(); ++i, ++val)
    {
        ASSERT_TRUE(*i == val);
    }

    val = 2;
    for (int_vector_t::const_reverse_iterator i = v.crbegin(); i != v.crend(); ++i, --val)
    {
        ASSERT_TRUE(*i == val);
    }


    std::cerr << "test of vector<int> passed" << std::endl;
}


} // namespace

void vector_test()
{
    common_test();
    test_vector_int();
}
