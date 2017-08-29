#include "tests/vector/vector_test.hpp"

#include <nestl/algorithm.hpp>

namespace nestl
{
namespace test
{

NESTL_ADD_TEST(vector_test)
{
    // assign zero elements to empty vector
    {
        vector<int> vec;

        NESTL_CHECK_OPERATION(vec.assign_nothrow(_, 0));

        CheckVectorSize(vec, 0);
    }

    // assign one default params to vector
    {
        const size_t expectedSize = 1;
        vector<int> vec;
        int defaultParam = 0;

        NESTL_CHECK_OPERATION(vec.assign_nothrow(_, expectedSize, defaultParam));

        CheckVectorSize(vec, expectedSize);

        NESTL_CHECK_EQ(defaultParam, *(vec.begin()));
        NESTL_CHECK_EQ(defaultParam, *(vec.cbegin()));
        NESTL_CHECK_EQ(defaultParam, vec[0]);
        NESTL_CHECK_EQ(defaultParam, vec.front());
        NESTL_CHECK_EQ(defaultParam, vec.back());
    }

    // assign several default params to vector
    {
        const size_t expectedSize = 1024;
        vector<int> vec;
        int defaultParam = 0;

        NESTL_CHECK_OPERATION(vec.assign_nothrow(_, expectedSize, defaultParam));

        CheckVectorSize(vec, expectedSize);

        vector<int>::const_iterator constIt = vec.cbegin();
        vector<int>::iterator it = vec.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(defaultParam, *constIt);
            NESTL_CHECK_EQ(defaultParam, *it);
            NESTL_CHECK_EQ(defaultParam, vec[i]);

            ++constIt;
            ++it;
        }
    }

    // insert one element into empty vector
    {
        vector<int> vec;
        int val = 0;

        NESTL_CHECK_OPERATION(vec.insert_nothrow(_, vec.begin(), val));

        CheckVectorSize(vec, 1);

        NESTL_CHECK_EQ(val, *(vec.begin()));
        NESTL_CHECK_EQ(val, *(vec.cbegin()));
        NESTL_CHECK_EQ(val, vec[0]);
        NESTL_CHECK_EQ(val, vec.front());
        NESTL_CHECK_EQ(val, vec.back());
    }


    ////////////////////////////////////////////////////////////////////////////////

    {
        typedef vector<int>::const_iterator const_iterator;
        typedef vector<int>::iterator       iterator;

        vector<int> vec;
        int values [] = {0, 1, 2, 3, 4, 5};
        const size_t expectedSize = std::distance(std::begin(values), std::end(values));

        NESTL_CHECK_OPERATION(vec.assign_nothrow(_, std::begin(values), std::end(values)));

        CheckVectorSize(vec, expectedSize);

        const_iterator constIt = vec.cbegin();
        iterator it = vec.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(values[i], *constIt);
            NESTL_CHECK_EQ(values[i], *it);
            NESTL_CHECK_EQ(values[i], vec[i]);

            ++constIt;
            ++it;
        }
    }


    {
        typedef vector<int>::iterator iterator_t;
        vector<int> vec;

        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 1));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 2));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 3));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 4));

        iterator_t st;
        NESTL_CHECK_OPERATION(st = vec.insert_nothrow(_, vec.begin(), 0););
        CheckVectorSize(vec, 5);
        NESTL_CHECK_EQ(vec.begin(), st);

        NESTL_CHECK_EQ(0, vec[0]);
        NESTL_CHECK_EQ(1, vec[1]);
        NESTL_CHECK_EQ(2, vec[2]);
        NESTL_CHECK_EQ(3, vec[3]);
        NESTL_CHECK_EQ(4, vec[4]);
    }

    {
        typedef vector<int>::iterator iterator_t;
        vector<int> vec;

        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 0));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 1));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 2));
        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, 3));

        iterator_t st;
        NESTL_CHECK_OPERATION(st = vec.insert_nothrow(_, vec.end(), 4););
        CheckVectorSize(vec, 5);
        NESTL_CHECK_EQ(vec.end() - 1, st);

        NESTL_CHECK_EQ(0, vec[0]);
        NESTL_CHECK_EQ(1, vec[1]);
        NESTL_CHECK_EQ(2, vec[2]);
        NESTL_CHECK_EQ(3, vec[3]);
        NESTL_CHECK_EQ(4, vec[4]);
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        vector<int> vec;
        int val = 0;

        NESTL_CHECK_OPERATION(vec.push_back_nothrow(_, val));

        CheckVectorSize(vec, 1);
    }


    {
        vector<int> vec;
        int val = 0;

        NESTL_CHECK_OPERATION(vec.emplace_back_nothrow(_, val));

        CheckVectorSize(vec, 1);
    }
}

} // namespace test
} // namespace nestl
