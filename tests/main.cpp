#include "tests.h"





allocation_count NonCopyableButAssignCopyable::ms_count;

allocation_count NonCopyableButMoveable::ms_count;

allocation_count CopyableButNotMoveable::ms_count;


bool NonCopyableButAssignCopyable::return_error_on_assign = false;

int main(int argc, char* argv [])
{
    vector_test();
    shared_ptr_test();
    algorithm_test();

    std::cerr << "\nAll tests successfully passed" << std::endl;

    return 0;
}
