#include "tests/list/list_test.hpp"


namespace nestl
{
namespace test
{

NESTL_ADD_TEST(list_test)
{
    {
        list<int> l;

        CheckListSize(l, 0);
    }

    {
        list<int> l;
        NESTL_CHECK_OPERATION(l.assign_nothrow(_, 0));

        CheckListSize(l, 0);
    }

    {
        const size_t expectedSize = 1;
        list<int> l;

        int defaultParam = 0;
        NESTL_CHECK_OPERATION(l.assign_nothrow(_, expectedSize, defaultParam));

        CheckListSize(l, expectedSize);

        NESTL_CHECK_EQ(defaultParam, *(l.begin()));
        NESTL_CHECK_EQ(defaultParam, *(l.cbegin()));
        NESTL_CHECK_EQ(defaultParam, l.front());
        NESTL_CHECK_EQ(defaultParam, l.back());
    }

    {
        const size_t expectedSize = 1024;
        list<int> l;
        int defaultParam = 0;

        NESTL_CHECK_OPERATION(l.assign_nothrow(_, expectedSize, defaultParam));

        CheckListSize(l, expectedSize);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(defaultParam, *constIt);
            NESTL_CHECK_EQ(defaultParam, *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        int val = 0;

        NESTL_CHECK_OPERATION(l.insert_nothrow(_, l.begin(), val));

        CheckListSize(l, 1);

        NESTL_CHECK_EQ(val, *(l.begin()));
        NESTL_CHECK_EQ(val, *(l.cbegin()));
        NESTL_CHECK_EQ(val, l.front());
        NESTL_CHECK_EQ(val, l.back());
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        list<int> l;
        int values [] = {0, 1, 2, 3, 4, 5};
        const size_t expectedSize = nestl::distance(std::begin(values), std::end(values));

        NESTL_CHECK_OPERATION(l.assign_nothrow(_, std::begin(values), std::end(values)));

        CheckListSize(l, expectedSize);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(values[i], *constIt);
            NESTL_CHECK_EQ(values[i], *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        const size_t expectedSize = 5;

        int values [] = {1, 2, 3, 4};

        NESTL_CHECK_OPERATION(l.assign_nothrow(_, std::begin(values), std::end(values)));

        list<int>::iterator st;
        NESTL_CHECK_OPERATION(st = l.insert_nothrow(_, l.begin(), 0));

        CheckListSize(l, expectedSize);
        NESTL_CHECK_EQ(l.begin(), st);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(static_cast<int>(i), *constIt);
            NESTL_CHECK_EQ(static_cast<int>(i), *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;
        const size_t expectedSize = 5;

        int values [] = {0, 1, 2, 3};
        NESTL_CHECK_OPERATION(l.assign_nothrow(_, std::begin(values), std::end(values)));

        list<int>::iterator st;
        NESTL_CHECK_OPERATION(st = l.insert_nothrow(_, l.end(), 4));
        CheckListSize(l, expectedSize);

        list<int>::iterator beforeEnd = l.end(); --beforeEnd;
        NESTL_CHECK_EQ(beforeEnd, st);

        list<int>::const_iterator constIt = l.cbegin();
        list<int>::iterator it = l.begin();
        for (size_t i = 0; i != expectedSize; ++i)
        {
            NESTL_CHECK_EQ(static_cast<int>(i), *constIt);
            NESTL_CHECK_EQ(static_cast<int>(i), *it);

            ++constIt;
            ++it;
        }
    }

    {
        list<int> l;

        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 0));
        CheckListSize(l, 1);

        l.pop_front();
        CheckListSize(l, 0);
    }

    {
        list<int> l;

        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 0));
        CheckListSize(l, 1);

        l.pop_back();
        CheckListSize(l, 0);
    }

    {
        list<int> l;

        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 0));
        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 1));
        CheckListSize(l, 2);

        l.pop_front();
        CheckListSize(l, 1);
        NESTL_CHECK_EQ(1, l.front());
    }

    {
        list<int> l;

        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 0));
        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, 1));
        CheckListSize(l, 2);

        l.pop_back();
        CheckListSize(l, 1);
        NESTL_CHECK_EQ(0, l.front());
    }


    {
        int v1 [] = {0, 2, 4, 6};

        list<int> l1;
        NESTL_CHECK_OPERATION(l1.assign_nothrow(_, std::begin(v1), std::end(v1)));

        int v2 [] = {1, 3, 5, 7};
        list<int> l2;
        NESTL_CHECK_OPERATION(l2.assign_nothrow(_, std::begin(v2), std::end(v2)));

        l1.merge(l2);
        CheckListSize(l2, 0);

        CheckListSize(l1, 8);

        list<int>::iterator it = l1.begin();
        for (int i = 0; i != 8; ++i, ++it)
        {
            NESTL_CHECK_EQ(i, *it);
        }
    }

    {
        list<int> l;

        l.sort();
        CheckListSize(l, 0);
    }

    {
        list<int> l;
        NESTL_CHECK_OPERATION(l.assign_nothrow(_, 1));

        l.sort();
        CheckListSize(l, 1);
        NESTL_CHECK_EQ(0, l.front());
    }

    {
        int unsorted [] = {0, 4, 2, 3, 1};

        list<int> l;
        NESTL_CHECK_OPERATION(l.assign_nothrow(_, std::begin(unsorted), std::end(unsorted)));

        l.sort();
        CheckListSize(l, 5);

        list<int>::iterator it = l.begin();
        for (int i = 0; i != 5; ++i, ++it)
        {
            NESTL_CHECK_EQ(i, *it);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    {
        list<int> l;
        int val = 0;

        NESTL_CHECK_OPERATION(l.push_back_nothrow(_, val));

        CheckListSize(l, 1);
    }


    {
        list<int> l;
        int val = 0;

        NESTL_CHECK_OPERATION(l.emplace_back_nothrow(_, val));

        CheckListSize(l, 1);
    }


    {
        list<int> l;
        int val = 0;

        NESTL_CHECK_OPERATION(l.push_front_nothrow(_, val));

        CheckListSize(l, 1);
    }

    {
        list<int> l;
        int val = 0;

        NESTL_CHECK_OPERATION(l.emplace_front_nothrow(_, val));

        CheckListSize(l, 1);
    }

#if NESTL_HAS_EXCEPTIONS

    {
        list<int> l;
        l.push_back(42);

        CheckListSize(l, 1);
    }

    {
        list<int> l1;
        l1.push_back(42);

        list<int> l2(l1);

        CheckListSize(l2, 1);
    }

#endif /* NESTL_HAS_EXCEPTIONS */
}


} // namespace test
} // namespace nestl
