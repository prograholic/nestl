nestl
=====

Design and implementation of some of stl containers and smart pointers without exception support.

**This implementation is not production ready yet**


How to build
============
Library is header-only. Library detects version of c++ (03, 11) and configures itself according to c++ version.
Also it is possible to add definition **NESTL_CONFIG_HAS_CXX11** for using c++11 features


How to use NESTL without standard library
-----------------------------------------
It is possible to use NESTL without standard library.
User should define macro **NESTL_NO_STD**
and provide platform-specific header (should define **NESTL_PLATFORM_HEADER**).

You can see nestl/platform.hpp.sample for details.
Also you can see tests/CMakeLists.txt and tests/sample_test_platform.hpp for details


Compilers supported
===================

Library has been tested on following platforms and compilers:

Toolchain                 |C++-03|C++-03-no-std|C++-11     |C++-11-no-std
--------------------------|------|-------------|-----------|-------------
i386-linux-gnu G++-4.7.4  |Yes   |Yes*         |Yes        |Yes*
i386-linux-gnu G++-4.8.3  |Yes   |Yes*         |Yes        |Yes*
i386-linux-gnu G++-4.9.1  |Yes   |Yes*         |Yes        |Yes*
i386-linux-gnu clang++-3.4|Yes   |Yes*         |Yes        |Yes*
i386-linux-gnu clang++-3.5|Yes   |Yes*         |Yes        |Yes
i386-windows msvc-2008    |Yes   |Yes*         |Yes**      |Yes***

\* - Tests successfully compiles but not executed (because of gtest/gmock emulation)

\*\* - Compiles, but actually this mode is same as C++-03

\*\*\* - Tests compiles but not executed and mode is equal to C++-03
