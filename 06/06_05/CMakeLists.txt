cmake_minimum_required(VERSION 3.28)
project(DynamicLibraryLoader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.85.0 REQUIRED)

add_executable(loader main.cpp)

add_library(test_v1 SHARED test_v1.cpp)
add_library(test_v2 SHARED test_v2.cpp)