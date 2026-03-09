cmake_minimum_required(VERSION 3.28)
project(QuickSortTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(sort_test 
    sort_test.cpp
)

target_link_libraries(sort_test 
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock
)

enable_testing()
add_test(NAME QuickSortTest COMMAND sort_test)
