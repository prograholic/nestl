#include "tests/shared_ptr/shared_ptr_test.hpp"


namespace nestl
{
namespace test
{

NESTL_ADD_TEST(shared_ptr_test)
{
    {
        shared_ptr<int> sp;

        NESTL_CHECK_OPERATION(sp = make_shared_nothrow<int>(_));
        CheckSharedPtr(sp, 1);
    }
}

} // namespace test
} // namespace nestl
