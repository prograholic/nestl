[![Build Status](https://travis-ci.org/prograholic/nestl.svg?branch=master)](https://travis-ci.org/prograholic/nestl)
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

How to run tests
----------------
Nestl needs following dependencies for running tests:
* CMake (http://www.cmake.org/), minimal version is 3.0
* googlemock sources (https://code.google.com/p/googlemock/), minimal version 1.7 (1.6 should be acceptable but not tested)

For configuring building and running nestl test suite one should perform following commands:
```sh
mkdir build
cd build
cmake ../ -DNESTL_GOOGLEMOCK_DIR=/absolute/path/to/googlemock/
make
make test
```



Compilers supported
===================

Library has been tested on following platforms and compilers:

Toolchain                    |C++-03|C++-03-no-std|C++-11     |C++-11-no-std
-----------------------------|------|-------------|-----------|-------------
x86_64-linux-gnu G++-4.6.3   |Yes   |Yes          |Yes        |Yes
i386-linux-gnu G++-4.7.4     |Yes   |Yes          |Yes        |Yes
i386-linux-gnu G++-4.8.3     |Yes   |Yes          |Yes        |Yes
i386-linux-gnu G++-4.9.1     |Yes   |Yes          |Yes        |Yes
x86_64-linux-gnu clang++-3.4 |Yes   |Yes          |Yes        |Yes
i386-linux-gnu clang++-3.4   |Yes   |Yes          |Yes        |Yes
i386-linux-gnu clang++-3.5   |Yes   |Yes          |Yes        |Yes
i386-windows msvc-2008       |Yes   |Yes          |Yes*       |Yes*

\* - Compiles, but actually this mode is same as C++-03
