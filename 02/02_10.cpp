////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////

void order(std::vector<int> &vector, std::size_t left, std::size_t right) {

    for (auto i = left + 1; i < right; ++i) {

        for (auto j = i; j > left; --j) {

            if (vector[j - 1] > vector[j]) {

                std::swap(vector[j], vector[j - 1]);
            }
            else {

                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

std::size_t median_of_three_index(const std::vector<int> &v, std::size_t left, std::size_t right) {

    std::size_t mid = std::midpoint(left, right - 1);

    int v_left = v[left], v_mid = v[mid], v_right = v[right - 1];

    if ((v_left <= v_mid && v_mid <= v_right) || (v_right <= v_mid && v_mid <= v_left))     return mid;
    if ((v_mid <= v_left && v_left <= v_right) || (v_right <= v_left && v_left <= v_mid))   return left;

    return right - 1;
}

////////////////////////////////////////////////////////////////////////////////////

static std::size_t hoare_partition(std::vector<int> &v, std::size_t left, std::size_t right) {

    using diff_t = std::ptrdiff_t;
    diff_t lo = static_cast<diff_t>(left);
    diff_t hi = static_cast<diff_t>(right) - 1;

    std::size_t mid_idx = median_of_three_index(v, left, right);
    int pivot = v[mid_idx];

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

////////////////////////////////////////////////////////////////////////////////////

void quick_split(std::vector<int> &v, std::size_t left, std::size_t right)
{

    constexpr std::size_t INSERTION_THRESHOLD = 16;

    if (right - left <= INSERTION_THRESHOLD) {

        order(v, left, right);
        return;  
    }

    std::size_t p = hoare_partition(v, left, right);

    if (p > left)   quick_split(v, left, p);
    if (p < right)  quick_split(v, p, right);
}

////////////////////////////////////////////////////////////////////////////////////

void sort(std::vector<int> &vector) {
    
    if (!vector.empty()) {

        quick_split(vector, 0, std::size(vector));
    }
}

////////////////////////////////////////////////////////////////////////////////////

int main() {
    
    std::size_t size = 1000;
    std::vector<int> vector(size, 0);

    for (auto i = 0uz; i < size; ++i) {

        vector[i] = static_cast<int>(size - i);
    }

    sort(vector);

    assert(std::ranges::is_sorted(vector));
}