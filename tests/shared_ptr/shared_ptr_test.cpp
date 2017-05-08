#include "tests/shared_ptr/shared_ptr_test.hpp"


namespace nestl
{
namespace test
{

void run_tests()
{
    {
        shared_ptr<int> sp;

        ASSERT_OPERATION_SUCCESS(sp = make_shared_nothrow<int>(_));
        CheckSharedPtr(sp, 1);
    }
}

} // namespace test
} // namespace nestl

int main()
{
    nestl::test::run_tests();
}
