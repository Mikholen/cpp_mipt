#include <gtest/gtest.h>
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <ranges>
#include <random>

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
static std::size_t hoare_partition(std::vector<T> &v, std::size_t left, std::size_t right) {
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

template <typename T>
void quick_split(std::vector<T> &v, std::size_t left, std::size_t right) {
    constexpr std::size_t INSERTION_THRESHOLD = 16;
    
    if (right - left <= INSERTION_THRESHOLD) {
        order(v, left, right);
        return;
    }
    
    std::size_t p = hoare_partition(v, left, right);
    
    if (p > left) quick_split(v, left, p);
    if (p < right) quick_split(v, p, right);
}

template <typename T>
void sort(std::vector<T> &vector) {
    if (!vector.empty()) {
        quick_split(vector, 0, std::size(vector));
    }
}

class QuickSortTest : public ::testing::Test {
protected:
    void SetUp() override {
        shuffled.resize(100);
        reverse_sorted.resize(100);
        sorted.resize(100);
        
        std::iota(shuffled.begin(), shuffled.end(), 0);
        std::ranges::shuffle(shuffled, std::mt19937{std::random_device{}()});
        std::generate(reverse_sorted.begin(), reverse_sorted.end(), [i=99]() mutable { return i--; });
        std::iota(sorted.begin(), sorted.end(), 0);
    }
    
    std::vector<int> shuffled, reverse_sorted, sorted;
};

TEST_F(QuickSortTest, EmptyVector) {
    std::vector<int> empty;
    sort(empty);
    EXPECT_TRUE(std::ranges::is_sorted(empty));
}

TEST_F(QuickSortTest, SingleElement) {
    std::vector<int> single = {42};
    sort(single);
    EXPECT_TRUE(std::ranges::is_sorted(single));
}

TEST_F(QuickSortTest, AlreadySorted) {
    auto test_copy = sorted;
    sort(test_copy);
    EXPECT_TRUE(std::ranges::is_sorted(test_copy));
}

TEST_F(QuickSortTest, ReverseSorted) {
    auto test_copy = reverse_sorted;
    sort(test_copy);
    EXPECT_TRUE(std::ranges::is_sorted(test_copy));
}

TEST_F(QuickSortTest, ShuffledVector) {
    auto test_copy = shuffled;
    sort(test_copy);
    EXPECT_TRUE(std::ranges::is_sorted(test_copy));
}

TEST_F(QuickSortTest, Duplicates) {
    std::vector<int> duplicates = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sort(duplicates);
    EXPECT_TRUE(std::ranges::is_sorted(duplicates));
}

TEST_F(QuickSortTest, SmallVectors) {
    std::vector<int> small = {5, 2, 8, 1, 9, 3};
    sort(small);
    EXPECT_TRUE(std::ranges::is_sorted(small));
}

TEST(QuickSortDouble, WorksWithDouble) {
    std::vector<double> doubles = {3.14, 2.71, 1.41, 1.73, 0.0, 2.0};
    sort(doubles);
    EXPECT_TRUE(std::ranges::is_sorted(doubles));
}

using QuickSortSizeTest = ::testing::TestWithParam<std::size_t>;

TEST_P(QuickSortSizeTest, DifferentSizes) {
    std::vector<int> vec(GetParam());
    std::iota(vec.begin(), vec.end(), 0);
    std::ranges::shuffle(vec, std::mt19937{std::random_device{}()});
    sort(vec);
    EXPECT_TRUE(std::ranges::is_sorted(vec));
}

INSTANTIATE_TEST_SUITE_P(Sizes, QuickSortSizeTest, 
    ::testing::Values(0, 1, 10, 100, 1000));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
