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



using HashFunc = std::size_t(*)(const std::string &);

std::vector<HashFunc> funcs = {
    h1,h2,h3,h4,h5,h6,h7,h8,h9
};

std::string gen_string(int i)
{
    return "str_" + std::to_string(i * 2654435761u);
}


std::size_t count_collisions(HashFunc f, int N)
{
    std::unordered_set<std::size_t> set;
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

    std::cout << "Data written to hash_data.csv\n";
}