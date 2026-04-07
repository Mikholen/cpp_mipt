#include <vector>
#include <iostream>
#include <iomanip>

int main()
{
    std::vector<int> v;

    std::size_t prev_capacity = 0;

    double sum_ratio = 0.0;
    std::size_t count = 0;

    for (std::size_t i = 0; i < 100000; ++i)
    {
        v.push_back(static_cast<int>(i));

        if (v.capacity() != prev_capacity)
        {
            if (prev_capacity != 0)
            {
                double ratio = double(v.capacity()) / double(prev_capacity);
                sum_ratio += ratio;
                ++count;

                std::cout << "grow: " << prev_capacity
                          << " -> " << v.capacity()
                          << " ratio=" << ratio << "\n";
            }

            prev_capacity = v.capacity();
        }
    }

    std::cout << "\naverage growth ratio = "
              << (sum_ratio / count) << "\n";
}