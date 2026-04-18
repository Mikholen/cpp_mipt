#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <functional>
#include <fstream>
#include <tuple>
#include <numeric>

#include <boost/container_hash/hash.hpp>

using namespace std::literals;


std::size_t h1(const std::string &s)
{
    return std::hash<std::string>{}(s);
}

std::size_t h2(const std::string &s)
{
    return boost::hash<std::string>{}(s);
}

std::size_t h3(const std::string &s)
{
    std::size_t seed = 0;
    for (char c : s)
        seed = seed * 31 + c;
    return seed;
}

std::size_t h4(const std::string &s)
{
    std::size_t seed = 0;
    for (char c : s)
        seed ^= (seed << 5) + (seed >> 2) + c;
    return seed;
}

std::size_t h5(const std::string &s)
{
    return std::hash<std::string_view>{}(s);
}

std::size_t h6(const std::string &s)
{
    std::size_t seed = 1469598103934665603ull;
    for (char c : s)
    {
        seed ^= (std::size_t)c;
        seed *= 1099511628211ull;
    }
    return seed;
}

std::size_t h7(const std::string &s)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, s);
    return seed;
}

// better to  use unsigned int over size_t
std::size_t h8(const std::string &s)
{
    return std::accumulate(s.begin(), s.end(), 0u);
}

std::size_t h9(const std::string &s)
{
    std::size_t seed = 0;
    for (char c : s)
        seed = (seed << 7) ^ (seed >> 3) ^ c;
    return seed;
}

// What functions are these? The task is to take the 9 functions from here: https://www.partow.net/programming/hashfunctions/index.html

using HashFunc = std::size_t(*)(const std::string &);

std::vector<HashFunc> funcs = {
    h1,h2,h3,h4,h5,h6,h7,h8,h9
};

std::string gen_string(int i) // You need to make a string with a-z characters, you need to pick the function from the Template: https://github.com/i-s-m-mipt/Education/blob/master/projects/examples/source/10.42.cpp
{
    return "str_" + std::to_string(i * 2654435761u);
}


std::size_t count_collisions(HashFunc f, int N)
{
    std::unordered_set<std::size_t> set; // task said to use 32 bit only, better to use unsigned int 
    std::size_t collisions = 0;

    for (int i = 0; i < N; ++i)
    {
        auto h = f(gen_string(i));
        if (!set.insert(h).second)
            ++collisions;
    }

    return collisions;
}


int main()
{
    std::ofstream out("hash_data.csv");
    out << "n,h1,h2,h3,h4,h5,h6,h7,h8,h9\n";

    for (int n = 1000; n <= 20000; n += 1000)
    {
        out << n;

        for (auto f : funcs)
            out << "," << count_collisions(f, n);

        out << "\n";
    }

    std::cout << "Data written to hash_data.csv\n"; // You need to attach python code
    // Use the python code to plot the hash_data.csv
    // Attach the .png file
    // And in this code write the analysis and conclusion
    // Comparing different names not h1 h2 h3 ... h9
}

// Score is 3/10
// You can resubmit and get upto 8/10, if you want
