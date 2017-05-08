#include "tests/vector/vector_test.hpp"

#include <nestl/algorithm.hpp>

namespace nestl
{
namespace test
{

void run_tests()
{
    {
        vector<int> vec;

        ASSERT_OPERATION_SUCCESS(vec.assign_nothrow(ec, 0));

        CheckVectorSize(vec, 0);
    }

    {
        const size_t expectedSize = 1;
        vector<int> vec;
        int defaultParam = 0;

        ASSERT_OPERATION_SUCCESS(vec.assign_nothrow(ec, expectedSize, defaultParam));

        CheckVectorSize(vec, expectedSize);

        EXPECT_EQ(defaultParam, *(vec.begin()));
        EXPECT_EQ(defaultParam, *(vec.cbegin()));
        EXPECT_EQ(defaultParam, vec[0]);
        EXPECT_EQ(defaultParam, vec.front());
        EXPECT_EQ(defaultParam, vec.back());
    }

    {
        const size_t expectedSize = 1024;
        vector<int> vec;
        int defaultParam = 0;

        ASSERT_OPERATION_SUCCESS(vec.assign_nothrow(ec, expectedSize, defaultParam));

        CheckVectorSize(vec, expectedSize);

        vector<int>::const_iterator constIt = vec.cbegin();
        vector<int>::iterator it = vec.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(defaultParam, *constIt);
            EXPECT_EQ(defaultParam, *it);
            EXPECT_EQ(defaultParam, vec[i]);

            ++constIt;
            ++it;
        }
    }

    {
        vector<int> vec;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(vec.insert_nothrow(ec, vec.begin(), val));

        CheckVectorSize(vec, 1);

        EXPECT_EQ(val, *(vec.begin()));
        EXPECT_EQ(val, *(vec.cbegin()));
        EXPECT_EQ(val, vec[0]);
        EXPECT_EQ(val, vec.front());
        EXPECT_EQ(val, vec.back());
    }


    ////////////////////////////////////////////////////////////////////////////////

    {
        typedef vector<int>::const_iterator const_iterator;
        typedef vector<int>::iterator       iterator;

        vector<int> vec;
        int values [] = {0, 1, 2, 3, 4, 5};
        const size_t expectedSize = nestl::distance(std::begin(values), std::end(values));

        ASSERT_OPERATION_SUCCESS(vec.assign_nothrow(ec, std::begin(values), std::end(values)));

        CheckVectorSize(vec, expectedSize);

        const_iterator constIt = vec.cbegin();
        iterator it = vec.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(values[i], *constIt);
            EXPECT_EQ(values[i], *it);
            EXPECT_EQ(values[i], vec[i]);

            ++constIt;
            ++it;
        }
    }


    {
        typedef vector<int>::iterator iterator_t;
        vector<int> vec;

        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 1));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 2));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 3));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 4));

        iterator_t st;
        ASSERT_OPERATION_SUCCESS(st = vec.insert_nothrow(ec, vec.begin(), 0););
        CheckVectorSize(vec, 5);
        EXPECT_EQ(vec.begin(), st);

        EXPECT_EQ(0, vec[0]);
        EXPECT_EQ(1, vec[1]);
        EXPECT_EQ(2, vec[2]);
        EXPECT_EQ(3, vec[3]);
        EXPECT_EQ(4, vec[4]);
    }

    {
        typedef vector<int>::iterator iterator_t;
        vector<int> vec;

        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 0));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 1));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 2));
        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, 3));

        iterator_t st;
        ASSERT_OPERATION_SUCCESS(st = vec.insert_nothrow(ec, vec.end(), 4););
        CheckVectorSize(vec, 5);
        EXPECT_EQ(vec.end() - 1, st);

        EXPECT_EQ(0, vec[0]);
        EXPECT_EQ(1, vec[1]);
        EXPECT_EQ(2, vec[2]);
        EXPECT_EQ(3, vec[3]);
        EXPECT_EQ(4, vec[4]);
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        vector<int> vec;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(vec.push_back_nothrow(ec, val));

        CheckVectorSize(vec, 1);
    }


    {
        vector<int> vec;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(vec.emplace_back_nothrow(ec, val));

        CheckVectorSize(vec, 1);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
