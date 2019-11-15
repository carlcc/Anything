#!/bin/bash

JOB_PARALLEL=4
cd 3rdparty
THIRD_PARTY_ROOT=$(pwd)

cd ${THIRD_PARTY_ROOT}
tar xfp cppfs-v1.3.0.tar.gz
cd cppfs-1.3.0/
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}" -DOPTION_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
make -j ${JOB_PARALLEL}
make install

cd ${THIRD_PARTY_ROOT}
tar xfp fmt-6.0.0.tar.gz
cd fmt-6.0.0
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}" -DBUILD_SHARED_LIBS=OFF -DFMT_TEST=OFF -DCMAKE_BUILD_TYPE=Release
make -j ${JOB_PARALLEL}
make install

cd ${THIRD_PARTY_ROOT}
tar xfp jsoncpp-1.9.1.tar.gz 
cd jsoncpp-1.9.1/
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}" -DBUILD_SHARED_LIBS=OFF -DFMT_TEST=OFF -DCMAKE_BUILD_TYPE=Release
make -j ${JOB_PARALLEL}
make install

cd ${THIRD_PARTY_ROOT}
tar xfp pcre-8.43.tar.bz2
cd pcre-8.43
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}" -DBUILD_SHARED_LIBS=OFF -DFMT_TEST=OFF -DCMAKE_BUILD_TYPE=Release \
    -DPCRE_BUILD_TESTS=OFF -DPCRE_BUILD_PCREGREP=OFF -DPCRE_SUPPORT_UTF=On
make -j ${JOB_PARALLEL}
make install