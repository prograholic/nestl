#!/bin/bash -e


cd ../build




build ()
{
  cd $1
  make clean
  make -j7
  make test
  cd ../../
}



build c++-03/clang++-3.4
build c++-03/clang++-3.5
build c++-03/g++-4.7
build c++-03/g++-4.8
build c++-03/g++-4.9

build c++-11/clang++-3.4
build c++-11/clang++-3.5
build c++-11/g++-4.7
build c++-11/g++-4.8
build c++-11/g++-4.9



build c++-03-no-std/clang++-3.4
build c++-03-no-std/clang++-3.5
build c++-03-no-std/g++-4.7
build c++-03-no-std/g++-4.8
build c++-03-no-std/g++-4.9

build c++-11-no-std/clang++-3.4
build c++-11-no-std/clang++-3.5
build c++-11-no-std/g++-4.7
build c++-11-no-std/g++-4.8
build c++-11-no-std/g++-4.9



cd ../src