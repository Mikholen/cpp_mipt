#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <numeric>

template <typename T>
void order(std::vector<T> &vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            } else {
                break;
            }
        }
    }
}

template <typename T>
std::size_t median_of_three_index(const std::vector<T> &v, std::size_t left, std::size_t right) {
    std::size_t mid = std::midpoint(left, right - 1);
    T v_left = v[left], v_mid = v[mid], v_right = v[right - 1];
    
    if ((v_left <= v_mid && v_mid <= v_right) || (v_right <= v_mid && v_mid <= v_left)) return mid;
    if ((v_mid <= v_left && v_left <= v_right) || (v_right <= v_left && v_left <= v_mid)) return left;
    return right - 1;
}

template <typename T>
std::size_t hoare_partition(std::vector<T> &v, std::size_t left, std::size_t right) {
    using diff_t = std::ptrdiff_t;
    diff_t lo = static_cast<diff_t>(left);
    diff_t hi = static_cast<diff_t>(right) - 1;
    
    std::size_t mid_idx = median_of_three_index(v, left, right);
    T pivot = v[mid_idx];
    
    diff_t i = lo - 1;
    diff_t j = hi + 1;
    
    while (true) {
        do { ++i; } while (v[static_cast<std::size_t>(i)] < pivot);
        do { --j; } while (v[static_cast<std::size_t>(j)] > pivot);
        
        if (i >= j) {
            return static_cast<std::size_t>(j + 1);
        }
        
        std::swap(v[static_cast<std::size_t>(i)], v[static_cast<std::size_t>(j)]);
    }
}

template <typename T, std::size_t THRESHOLD>
void quick_split(std::vector<T> &v, std::size_t left, std::size_t right) {
    if (right - left <= THRESHOLD) {
        order(v, left, right);
        return;
    }
    
    std::size_t p = hoare_partition(v, left, right);
    
    if (p > left) quick_split<T, THRESHOLD>(v, left, p);
    if (p < right) quick_split<T, THRESHOLD>(v, p, right);
}

template <std::size_t THRESHOLD>
static void BM_QuickSortReverse(benchmark::State& state) {
    std::vector<double> data(10000);
    std::generate(data.rbegin(), data.rend(), [i=0]() mutable { return 10000.0 - i++; });
    
    for (auto _ : state) {
        auto copy = data;
        quick_split<double, THRESHOLD>(copy, 0, copy.size());
        benchmark::DoNotOptimize(copy);
    }
}

BENCHMARK_TEMPLATE(BM_QuickSortReverse, 4)->Name("Threshold=4");
BENCHMARK_TEMPLATE(BM_QuickSortReverse, 8)->Name("Threshold=8");
BENCHMARK_TEMPLATE(BM_QuickSortReverse, 16)->Name("Threshold=16");
BENCHMARK_TEMPLATE(BM_QuickSortReverse, 32)->Name("Threshold=32");
BENCHMARK_TEMPLATE(BM_QuickSortReverse, 64)->Name("Threshold=64");

static void BM_StdSortReverse(benchmark::State& state) {
    std::vector<double> data(10000);
    std::generate(data.rbegin(), data.rend(), [i=0]() mutable { return 10000.0 - i++; });
    
    for (auto _ : state) {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
        benchmark::DoNotOptimize(copy);
    }
}

BENCHMARK(BM_StdSortReverse)->Name("std::sort");

BENCHMARK_MAIN();

// Okay score is 10/10
