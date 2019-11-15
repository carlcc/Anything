cmake_minimum_required(VERSION 3.2)

project(anything)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall")

option(AT_ENABLE_QT_GUI "Enable GUI based on qt5" OFF)

include_directories(3rdparty/include)
link_directories(3rdparty/lib)

add_subdirectory(src)