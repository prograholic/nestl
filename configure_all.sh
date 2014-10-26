#!/bin/bash -e

cd ../

if [ ! -d build ]; then
  mkdir build
fi

cd build


configure()
{
  mkdir -p $1
  cd $1
  CC=$2 CXX=$3 cmake ../../../src -DCMAKE_BUILD_TYPE=Debug -DNESTL_GOOGLEMOCK_DIR=/usr/src/projects/googlecode/googlemock -DNESTL_DISABLE_STD=$4 -DNESTL_ENABLE_CXX11=$5
  cd ../../
}


# c++-03 with STD
configure c++-03/clang++-3.4        clang-3.4 clang++-3.4 No  No
configure c++-03/clang++-3.5        clang-3.5 clang++-3.5 No  No
configure c++-03/g++-4.7            gcc-4.7   g++-4.7     No  No
configure c++-03/g++-4.8            gcc-4.8   g++-4.8     No  No
configure c++-03/g++-4.9            gcc-4.9   g++-4.9     No  No

# c++-11 with STD
configure c++-11/clang++-3.4        clang-3.4 clang++-3.4 No  Yes
configure c++-11/clang++-3.5        clang-3.5 clang++-3.5 No  Yes
configure c++-11/g++-4.7            gcc-4.7   g++-4.7     No  Yes
configure c++-11/g++-4.8            gcc-4.8   g++-4.8     No  Yes
configure c++-11/g++-4.9            gcc-4.9   g++-4.9     No  Yes


# c++-03 without STD
configure c++-03-no-std/clang++-3.4 clang-3.4 clang++-3.4 Yes No
configure c++-03-no-std/clang++-3.5 clang-3.5 clang++-3.5 Yes No
configure c++-03-no-std/g++-4.7     gcc-4.7   g++-4.7     Yes No
configure c++-03-no-std/g++-4.8     gcc-4.8   g++-4.8     Yes No
configure c++-03-no-std/g++-4.9     gcc-4.9   g++-4.9     Yes No

# c++-11 without STD
configure c++-11-no-std/clang++-3.4 clang-3.4 clang++-3.4 Yes Yes
configure c++-11-no-std/clang++-3.5 clang-3.5 clang++-3.5 Yes Yes
configure c++-11-no-std/g++-4.7     gcc-4.7   g++-4.7     Yes Yes
configure c++-11-no-std/g++-4.8     gcc-4.8   g++-4.8     Yes Yes
configure c++-11-no-std/g++-4.9     gcc-4.9   g++-4.9     Yes Yes

cd ../src
