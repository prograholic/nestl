#include "tests.h"





allocation_count NonCopyableButAssignCopyable::ms_count;

allocation_count NonCopyableButMoveable::ms_count;

allocation_count CopyableButNotMoveable::ms_count;


int main(int argc, char* argv [])
{
    vector_test();
    shared_ptr_test();

    std::cerr << "\nAll tests successfully passed" << std::endl;

    return 0;
}
