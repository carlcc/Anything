cmake_minimum_required(VERSION 3.5)

project(anythinggui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(anythinggui
  main.cpp
  mainwindow.cpp
  mainwindow.h
  anythingitemmodel.cpp
  anythingitemmodel.h
  simplethreadsafequeue.h
)

target_include_directories(anythinggui PRIVATE
  ../../
)

target_link_libraries(anythinggui PRIVATE Qt5::Widgets anything fmt cppfs pcre pthread)
