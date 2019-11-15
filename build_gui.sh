#!/bin/bash

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DAT_ENABLE_QT_GUI=ON
make -j 4