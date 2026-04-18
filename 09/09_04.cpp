#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>


template <typename RandomIt>
void order(RandomIt first, RandomIt last) {

    if (first == last) return;

    for (auto it = std::next(first); it != last; ++it) {

        for (auto jt = it; jt != first; --jt) {

            auto prev = std::prev(jt);

            if (*prev > *jt) {

                std::iter_swap(prev, jt);
            }
            else {

                break;
            }
        }
    }
}


template <typename RandomIt>
RandomIt median_of_three(RandomIt first, RandomIt last) {

    auto mid = first;
    std::advance(mid, std::distance(first, last) / 2);

    auto last_elem = std::prev(last);

    if ((*first <= *mid && *mid <= *last_elem) ||
        (*last_elem <= *mid && *mid <= *first))
        return mid;

    if ((*mid <= *first && *first <= *last_elem) ||
        (*last_elem <= *first && *first <= *mid))
        return first;

    return last_elem;
}

template <typename RandomIt>
RandomIt hoare_partition(RandomIt first, RandomIt last) {

    auto pivot_it = median_of_three(first, last);
    auto pivot = *pivot_it;

    auto i = first;
    auto j = std::prev(last);

    while (true) {

        while (*i < pivot) ++i;
        while (*j > pivot) --j;

        if (i >= j)
            return std::next(j);

        std::iter_swap(i, j);

        ++i;
        --j;
    }
}


template <typename RandomIt>
void quick_split(RandomIt first, RandomIt last) {

    using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
    constexpr diff_t INSERTION_THRESHOLD = 16;
    if (std::distance(first, last) <= INSERTION_THRESHOLD) {

        order(first, last);
        return;
    }

    auto p = hoare_partition(first, last);

    if (first != p)
        quick_split(first, p);

    if (p != last)
        quick_split(p, last);
}

template <typename RandomIt>
void my_sort(RandomIt first, RandomIt last) {

    if (first != last) {

        quick_split(first, last);
    }
}

int main() {

    std::size_t size = 1000;

    std::vector<int> v(size);
    std::vector<double> v2(size);

    for (std::size_t i = 0; i < size; ++i) {

        v[i]  = static_cast<int>(size - i);
        v2[i] = static_cast<double>(size - i);
    }

    my_sort(v.begin(), v.end());
    my_sort(v2.begin(), v2.end());

    assert(std::ranges::is_sorted(v));
    assert(std::ranges::is_sorted(v2));
}

// Score is 9/10
