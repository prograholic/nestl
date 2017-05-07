#include "tests/set/set_test.hpp"

namespace nestl
{
namespace test
{

void run_tests()
{

    {
        nestl::set<int> s;

        s.insert(10);

        CheckSetSize(s, 1);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
