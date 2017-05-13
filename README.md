[![Build Status](https://travis-ci.org/prograholic/nestl.svg?branch=master)](https://travis-ci.org/prograholic/nestl)
nestl
=====

**TODO: translate description from russian to english**
**This implementation is not production ready yet**

Design and implementation of some of stl containers, smart pointers and algorithms without exception support.


Motivation
==========


```
nestl::vector<int> vec;
nestl::default_operation_error err;
vec.push_back_nothrow(err, 10);
if (err)
{
    // handle error somehow
    std::cerr << "error occured" << std::endl;
}
```


-----------------------------




-----------------------------------------

```
std::vector<int> x;
x.push_back(1);
```


```
std::vector<std::list<int>> m;
m.push_back(get_list_somehow());
```




1. exception handling - explicit error codes handling
2. Any constructor which may throw - two-phase initialization (default ctor + `init(args)`)
3. Any assignment operator which may throw - explicit `copy` method (for example `vec.copy_nothrow(err, other_vec);`)




------------------------



`template <typename HasException, typename NoException>
using dispatch = typename ::std::conditional<exception_support_t::value, HasExceptions, NoExceptions>::type
`



--------------------------



Error handling
--------------









Containers
----------


Type requirements and limitations
---------------------------------





How to build
============
Library is header-only. Library requires C++-11 compliant compiler and standard library.


How to use NESTL
-----------------------------------------

How to run tests
----------------
Nestl needs following dependencies for running tests:
* CMake (http://www.cmake.org/), minimal version is 3.0

For configuring building and running nestl test suite one should perform following commands:
```sh
mkdir build
cd build
cmake
make
make test
```



Compilers supported
===================

Library has been tested on following platforms and compilers:

Toolchain
-----------------------------
i386-linux-gnu G++-4.8.3
x86_64-linux-gnu clang++-3.4
i386-linux-gnu clang++-3.4
i386-linux-gnu clang++-3.5
i386-windows msvc-2013
i386-windows msvc-2015
i386-windows msvc-2017

