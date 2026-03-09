cmake_minimum_required(VERSION 3.28)
project(QuickSortBenchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark REQUIRED)

add_executable(quick_benchmark 
    quick_benchmark.cpp
)

target_link_libraries(quick_benchmark 
    benchmark::benchmark 
    benchmark::benchmark_main
)
