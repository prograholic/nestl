[![Build Status](https://travis-ci.org/prograholic/nestl.svg?branch=master)](https://travis-ci.org/prograholic/nestl)

[![Windows Build status](https://ci.appveyor.com/api/projects/status/github/prograholic/nestl)](https://ci.appveyor.com/project/prograholic/nestl)

[![Coverage Status](https://coveralls.io/repos/github/prograholic/nestl/badge.svg?branch=master)](https://coveralls.io/github/prograholic/nestl?branch=master)


nestl
=====

**TODO: translate description from russian to english**

**This implementation is not production ready yet**

Design and implementation of some of stl containers, smart pointers and algorithms without exception support.
This library allows to write common code for environments with exceptions and for environments without exceptions.
Therefore all operations which may perform in any modes are marked as `noexcept`.

Additionally this library provides extensions for containers and smart pointers for environments with exceptions.
Such extensions allow programmer to initialize, fill and work with containers in an usual way (as stl analogues).

Motivation
==========

Here is simple use case:

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


Для чего эта библиотека нужна
-----------------------------

Как известно, в C++ исключения - это краеугольный камень, так как исключения позволяют использовать RAII и т.д.
Но, с другой стороны, язык C++ является языком общего назначения,
и кроме всего прочего, используется как язык для системного программирования,
а в системном программирование отсутствие исключений скорее норма, нежели исключение.

Кроме этого, есть и другие области программирования, где использование исключение неоправданно, например, из-за производительности.
Например, в ОС Windows бросание исключения требует перехода в режим ядра, и бросок исключения фактически начинается из ядра.
Поэтому, бросание исключения может быть дорогой операцией.


Why stl is not suitable
-----------------------
Consider following example

```
std::vector<int> x;
x.push_back(1);
```

Such code may not complie, or may not link or may crash in runtime when excetions are disabled.
The main problem is how to know whether operation was succeed or not?
Well, we can call method `x.size()` and check that it returns `1`.
But such approach may be very difficult and inconvenient:
```
std::vector<std::list<int>> m;
m.push_back(get_list_somehow());
```
We should check vector and all elements in vector, because copying of std::list may fail too.

Alos implementation of std::vector assume that method allocator::allocate returns valid pointer (or throws exception).
How allocator can handle situation when it cannot allocate memory? It can return nullptr - but we can break implementation of std::vector.
Also allocator can invoke std::abort, but such approach is not user-friendly.

Therefore current implementation of standard library can be used in exceptionless environments until first error occurs. After that library cannot give us any guarantee.


Если переформулировать проблему, то в среде без исключений мы должны предоставить некие аналоги следующих механизмов:
1. exception handling - explicit error codes handling
2. Any constructor which may throw - two-phase initialization (default ctor + `init(args)`)
3. Any assignment operator which may throw - explicit `copy` method (for example `vec.copy_nothrow(err, other_vec);`)




Library modes
-------------

Library can work in two modes - when exceptions are enabled (has_exceptions) and when exceptions are disabled (no_exceptions).
Programmer may check for particular mode using macro `NESTL_HAS_EXCEPTIONS` it`s value can be 1 and 0 respectively.

Additionally there is special type
`nestl::exception_support_t` - when exceptions are enabled it`s equal to type std::true_type,
and std::false_type in case when exceptions are disabled

Also there is helper template
`template <typename HasException, typename NoException>
using dispatch = typename ::std::conditional<exception_support_t::value, HasExceptions, NoExceptions>::type
`
This template allows to select implementation depending on exception support.



Library structure
-----------------

We decouple containers, alocators and algorithms like STL does.
Moreover we explicitly decouple error handling because client may have it`s own error handling mechanism.


Error handling
--------------

We should use error codes since we cannot throw and catch excptions in exceptionless environments.

Library provides `OperationError` concept - objects of types which satifies this concept can store some information about error.
Client may check for error using `explicit operator bool()`

Library provides `default_operation_error` - it`s default error handling implementation for each mode.


Also programmer can provide it`s own operation error implementation.
Such operation error type should provide `operator bool` or it`s analogue.
This operator should return `true` in case when object contains error and `false` otherwise.

Кроме этого, библиотека требует наличия следующих функций:
Additionally library requires following functions (they should be accessible via ADL lookup)
`build_length_error` - function should initialize error (analogue of `std::length_error` exception)
`build_bad_alloc` - function should ininitialize error (analogue of `std::bad_alloc` exception)

For environments with exception support library additionally requires following functions:
`from_exception_ptr` - function should initialize error from `std::exception_ptr` object (should capture exception from exception_ptr somehow)
`from_exception` - function should initialize error from arbitrary exception object (should capture exception).
`throw_exception` - function should throw previously captured exception from given error obejct.


Containers
----------
Library provides analogues of standard containers (vector, list, set, map)


Emulating of object copying and non-trivial initialization
----------------------------------------------------------
Sometimes it is necessary to construct object with some non trivial initialization. For such cases library allows user to specialize type
`nestl::two_phase_initializator` and provide static method `init`
```
namespace nestl
{
template <>
struct two_phase_initializator <MyType>
{
    template <typename OperationError, typename ... Args>
    static void init(OperationError& err, MyType& defaultConstructed, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.CustomInit(err, std::forward<Args>(args)...);
    }
};
}
```
Implementation of method `init` may perform som non-trivial initialization of obejct.
Note that client should not destroy `defaultConstructed` object in case of failure. Library will handle such situation and properly destroys object



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
```
mkdir build
cd build
cmake
make
make test
```



Compilers supported
===================

Library has been tested on following platforms and compilers:

Toolchain list
--------------

| Platform        | Compiler          |
|-----------------|-------------------|
|i386-linux-gnu   | G++-4.8.3         |
|x86_64-linux-gnu | clang++-3.4       |
|i386-linux-gnu   | clang++-3.4       |
|i386-linux-gnu   | clang++-3.5       |
|i386-windows     | msvc-2013 *       |
|i386-windows     | msvc-2015         |
|i386-windows     | msvc-2017         |
|i386-windows     | mingw32-G++-5.3.0 |



--
`*` msvc-2013 does not support `noexcept` specifier, therefore we emulate it using `throw()` in nestl implementation
