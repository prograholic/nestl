#include <nestl/vector.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>


// gtest-like macro
#define ASSERT_TRUE(x) do {if (!(x)) {std::cerr << "assertion failed: " #x << " returns false" << std::endl; std::exit(EXIT_FAILURE);}} while (0)

#define ASSERT_SUCCESS(x) do {auto res = (x); if (res.value() != 0) {std::cerr << "assertion failed: " << #x << " returns [" << res.message() << "]" << std::endl; std::exit(EXIT_FAILURE);}} while (0)


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
void test_vector_ex()
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
}


template<size_t count>
struct assign_vector_constructor
{
    enum
    {
        expected_size = count
    };

    template <typename Vector>
    static std::error_condition construct(Vector& v)
    {
        ensure_vector_size(v, 0);

        return v.assign(count);
    }
};

template <typename T, typename A>
void test_vector(const char* name)
{
    std::cerr << "test [" << name << "] started" << std::endl;
    typedef nestl::vector<T, A> vector_t;

    {
        /// construct and destruct empty vector
        vector_t v;
        ensure_vector_size(v, 0);
    }

    test_vector_ex<vector_t, assign_vector_constructor<0>>();
    test_vector_ex<vector_t, assign_vector_constructor<1>>();
    test_vector_ex<vector_t, assign_vector_constructor<2>>();
    test_vector_ex<vector_t, assign_vector_constructor<100>>();

    std::cerr << "test [" << name << "] passed" << std::endl;
}




struct NonCopyableButAssignCopyable :  private nestl::noncopyable
{
    NonCopyableButAssignCopyable() noexcept
    {
        assert(ms_count >= 0);
        ++ms_count;
    }

    ~NonCopyableButAssignCopyable() noexcept
    {
        assert(ms_count > 0);
        --ms_count;
    }

    std::error_condition assign_copy(const NonCopyableButAssignCopyable& other) noexcept
    {
        return std::error_condition();
    }

    static int ms_count;
};

int NonCopyableButAssignCopyable::ms_count = 0;


struct NonCopyableButMoveable :  private nestl::noncopyable
{
    NonCopyableButMoveable() noexcept
    {
        assert(ms_count >= 0);
        ++ms_count;
    }

    NonCopyableButMoveable(NonCopyableButMoveable&& other) noexcept
    {
        assert(ms_count >= 0);
    }

    ~NonCopyableButMoveable() noexcept
    {
        assert(ms_count > 0);
        --ms_count;
    }

    static int ms_count;
};

int NonCopyableButMoveable::ms_count = 0;

struct TriviallyCopyable
{
    int val1;
    int val2;
};


int main(int argc, char* argv [])
{
    test_vector<int, nestl::allocator<int>>(
                "int,                                nestl::allocator<int>                         ");

    test_vector<NonCopyableButAssignCopyable, nestl::allocator<NonCopyableButAssignCopyable>>(
                "NonCopyableButAssignCopyable,       nestl::allocator<NonCopyableButAssignCopyable>");
    ASSERT_TRUE(NonCopyableButAssignCopyable::ms_count == 0);

    test_vector<NonCopyableButMoveable, nestl::allocator<NonCopyableButMoveable>>(
                "NonCopyableButMoveable,             nestl::allocator<NonCopyableButMoveable>      ");
    ASSERT_TRUE(NonCopyableButMoveable::ms_count == 0);

    test_vector<TriviallyCopyable, nestl::allocator<TriviallyCopyable>>(
                "TriviallyCopyable,                  nestl::allocator<TriviallyCopyable>           ");

    return 0;
}
