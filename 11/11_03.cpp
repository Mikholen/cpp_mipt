#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <ranges>
#include <functional>
#include <iostream>

template <typename RandomIt, typename Compare>
void order(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last) return;

    for (auto it = std::next(first); it != last; ++it)
    {
        for (auto jt = it; jt != first; --jt)
        {
            auto prev = std::prev(jt);

            if (comp(*jt, *prev))
            {
                std::iter_swap(prev, jt);
            }
            else
            {
                break;
            }
        }
    }
}

template <typename RandomIt, typename Compare>
RandomIt median_of_three(RandomIt first, RandomIt last, Compare comp)
{
    auto mid = first;
    std::advance(mid, std::distance(first, last) / 2);

    auto last_elem = std::prev(last);

    if (!comp(*mid, *first) && !comp(*last_elem, *mid)) return mid;
    if (!comp(*first, *mid) && !comp(*mid, *last_elem)) return first;
    return last_elem;
}

template <typename RandomIt, typename Compare>
RandomIt hoare_partition(RandomIt first, RandomIt last, Compare comp)
{
    auto pivot_it = median_of_three(first, last, comp);
    auto pivot = *pivot_it;

    auto i = first;
    auto j = std::prev(last);

    while (true)
    {
        while (comp(*i, pivot)) ++i;
        while (comp(pivot, *j)) --j;

        if (i >= j)
            return std::next(j);

        std::iter_swap(i, j);

        ++i;
        --j;
    }
}

template <typename RandomIt, typename Compare>
void quick_split(RandomIt first, RandomIt last, Compare comp)
{
    using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
    constexpr diff_t INSERTION_THRESHOLD = 16;

    if (std::distance(first, last) <= INSERTION_THRESHOLD)
    {
        order(first, last, comp);
        return;
    }

    auto p = hoare_partition(first, last, comp);

    if (first != p)
        quick_split(first, p, comp);

    if (p != last)
        quick_split(p, last, comp);
}

template <typename RandomIt, typename Compare>
void my_sort(RandomIt first, RandomIt last, Compare comp)
{
    if (first != last)
        quick_split(first, last, comp);
}

bool my_comp(int a, int b)
{
    return a < b;
}

int main()
{
    std::vector<int> v1 = {9, 1, 5, 3, 7, 2, 8};
    std::vector<int> v2 = v1;
    std::vector<int> v3 = v1;

    my_sort(v1.begin(), v1.end(), my_comp);
    my_sort(v2.begin(), v2.end(), std::less<int>{});
    my_sort(v3.begin(), v3.end(), [](int a, int b)
    {
        return a < b;
    });

    for (auto x : v1) std::cout << x << " ";
    std::cout << "\n";

    for (auto x : v2) std::cout << x << " ";
    std::cout << "\n";

    for (auto x : v3) std::cout << x << " ";
    std::cout << "\n";

    std::cout << std::boolalpha;
    std::cout << std::ranges::is_sorted(v1) << "\n";
    std::cout << std::ranges::is_sorted(v2) << "\n";
    std::cout << std::ranges::is_sorted(v3) << "\n";
}