nestl
=====

Design and implementation of some of stl containers and smart pointers without exception support.

**This implementation is not production ready and should be treated only as a proof of concept**


How to build
============

Library is header-only. Library detects version of c++ (03, 11) and configures itself according to c++ version.
Also it is possible to add definition **NESTL_ENABLE_CXX11** for using c++11 features



Compilers supported
===================

Library has been tested on g++-4.9.1 on linux
