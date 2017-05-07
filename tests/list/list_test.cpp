#include "tests/list/list_test.hpp"


namespace nestl
{
namespace test
{

void run_tests()
{
    {
        list<int> l;

        CheckListSize(l, 0);
    }

    {
        list<int> l;
        ASSERT_OPERATION_SUCCESS(l.assign(0));

        CheckListSize(l, 0);
    }

    {
        const size_t expectedSize = 1;
        list<int> l;

        int defaultParam = 0;
        ASSERT_OPERATION_SUCCESS(l.assign(expectedSize, defaultParam));

        CheckListSize(l, expectedSize);

        EXPECT_EQ(defaultParam, *(l.begin()));
        EXPECT_EQ(defaultParam, *(l.cbegin()));
        EXPECT_EQ(defaultParam, l.front());
        EXPECT_EQ(defaultParam, l.back());
    }

    {
        const size_t expectedSize = 1024;
        list<int> l;
        int defaultParam = 0;

        ASSERT_OPERATION_SUCCESS(l.assign(expectedSize, defaultParam));

        CheckListSize(l, expectedSize);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(defaultParam, *constIt);
            EXPECT_EQ(defaultParam, *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(l.insert(l.begin(), val));

        CheckListSize(l, 1);

        EXPECT_EQ(val, *(l.begin()));
        EXPECT_EQ(val, *(l.cbegin()));
        EXPECT_EQ(val, l.front());
        EXPECT_EQ(val, l.back());
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        list<int> l;
        int values [] = {0, 1, 2, 3, 4, 5};
        const size_t expectedSize = nestl::distance(std::begin(values), std::end(values));

        ASSERT_OPERATION_SUCCESS(l.assign(std::begin(values), std::end(values)));

        CheckListSize(l, expectedSize);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(values[i], *constIt);
            EXPECT_EQ(values[i], *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        const size_t expectedSize = 5;

        int values [] = {1, 2, 3, 4};

        ASSERT_OPERATION_SUCCESS(l.assign(std::begin(values), std::end(values)));

        list<int>::iterator_with_operation_error st = l.insert(l.begin(), 0);
        ASSERT_OPERATION_SUCCESS(st);

        CheckListSize(l, expectedSize);
        EXPECT_EQ(l.begin(), st.result());

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(static_cast<int>(i), *constIt);
            EXPECT_EQ(static_cast<int>(i), *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        const size_t expectedSize = 5;

        int values [] = {0, 1, 2, 3};
        ASSERT_OPERATION_SUCCESS(l.assign(std::begin(values), std::end(values)));

        list<int>::iterator_with_operation_error st = l.insert(l.end(), 4);
        ASSERT_OPERATION_SUCCESS(st);
        CheckListSize(l, expectedSize);

        list<int>::iterator beforeEnd = l.end(); --beforeEnd;
        EXPECT_EQ(beforeEnd, st.result());

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            EXPECT_EQ(static_cast<int>(i), *constIt);
            EXPECT_EQ(static_cast<int>(i), *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;

        ASSERT_OPERATION_SUCCESS(l.push_back(0));
        CheckListSize(l, 1);

        l.pop_front();
        CheckListSize(l, 0);
    }

    {
        list<int> l;

        ASSERT_OPERATION_SUCCESS(l.push_back(0));
        CheckListSize(l, 1);

        l.pop_back();
        CheckListSize(l, 0);
    }

    {
        list<int> l;

        ASSERT_OPERATION_SUCCESS(l.push_back(0));
        ASSERT_OPERATION_SUCCESS(l.push_back(1));
        CheckListSize(l, 2);

        l.pop_front();
        CheckListSize(l, 1);
        EXPECT_EQ(1, l.front());
    }

    {
        list<int> l;

        ASSERT_OPERATION_SUCCESS(l.push_back(0));
        ASSERT_OPERATION_SUCCESS(l.push_back(1));
        CheckListSize(l, 2);

        l.pop_back();
        CheckListSize(l, 1);
        EXPECT_EQ(0, l.front());
    }


    {
        int v1 [] = {0, 2, 4, 6};

        list<int> l1;
        ASSERT_OPERATION_SUCCESS(l1.assign(std::begin(v1), std::end(v1)));

        int v2 [] = {1, 3, 5, 7};
        list<int> l2;
        ASSERT_OPERATION_SUCCESS(l2.assign(std::begin(v2), std::end(v2)));

        l1.merge(l2);
        CheckListSize(l2, 0);

        CheckListSize(l1, 8);

        list<int>::iterator it = l1.begin();
        for (int i = 0; i != 8; ++i, ++it)
        {
            EXPECT_EQ(i, *it);
        }
    }

    {
        list<int> l;

        l.sort();
        CheckListSize(l, 0);
    }

    {
        list<int> l;
        ASSERT_OPERATION_SUCCESS(l.assign(1));

        l.sort();
        CheckListSize(l, 1);
        EXPECT_EQ(0, l.front());
    }

    {
        int unsorted [] = {0, 4, 2, 3, 1};

        list<int> l;
        ASSERT_OPERATION_SUCCESS(l.assign(std::begin(unsorted), std::end(unsorted)));

        l.sort();
        CheckListSize(l, 5);

        list<int>::iterator it = l.begin();
        for (int i = 0; i != 5; ++i, ++it)
        {
            EXPECT_EQ(i, *it);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        list<int> l;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(l.push_back(val));

        CheckListSize(l, 1);
    }


    {
        list<int> l;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(l.emplace_back(val));

        CheckListSize(l, 1);
    }


    {
        list<int> l;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(l.push_front(val));

        CheckListSize(l, 1);
    }

    {
        list<int> l;
        int val = 0;

        ASSERT_OPERATION_SUCCESS(l.emplace_front(val));

        CheckListSize(l, 1);
    }
}


} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
