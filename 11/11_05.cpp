#include <iostream>
#include <vector>
#include <list>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <tuple>

template<class R, class Out, class Pred, class Func>
Out transform_if(R&& range, Out out, Pred pred, Func func) {
    std::vector<std::ranges::range_value_t<R>> temp;

    std::ranges::copy_if(range, std::back_inserter(temp), pred);

    return std::ranges::transform(temp, out, func).out;
}

double mae(const std::vector<double>& actual, const std::vector<double>& predicted) {
    std::vector<double> errors(actual.size());

    std::transform(actual.begin(), actual.end(),
                   predicted.begin(),
                   errors.begin(),
                   [](double a, double p) {
                       return std::abs(a - p);
                   });

    return std::accumulate(errors.begin(), errors.end(), 0.0) / errors.size();
}

double mse(const std::vector<double>& actual, const std::vector<double>& predicted) {
    std::vector<double> errors(actual.size());

    std::transform(actual.begin(), actual.end(),
                   predicted.begin(),
                   errors.begin(),
                   [](double a, double p) {
                       double diff = a - p;
                       return diff * diff;
                   });

    return std::accumulate(errors.begin(), errors.end(), 0.0) / errors.size();
}

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
private:
    std::size_t count_;

    class Iterator {
    private:
        std::size_t index_ = 0;
        std::size_t max_;
        unsigned long long a_ = 0;
        unsigned long long b_ = 1;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = unsigned long long;
        using difference_type = std::ptrdiff_t;

        Iterator(std::size_t index, std::size_t max)
            : index_(index), max_(max) {}

        value_type operator*() const {
            return a_;
        }

        Iterator& operator++() {
            auto next = a_ + b_;
            a_ = b_;
            b_ = next;
            ++index_;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return index_ == other.index_;
        }
    };

public:
    explicit Fibonacci(std::size_t count) : count_(count) {}

    Iterator begin() const {
        return Iterator{0, count_};
    }

    Iterator end() const {
        return Iterator{count_, count_};
    }
};

int main() {
    std::vector<int> v{1,2,3,2,4,2,5};

    std::ranges::replace(v, 2, 9);
    std::ranges::fill(v.begin(), v.begin()+2, 0);
    auto unique_result = std::ranges::unique(v);
    v.erase(unique_result.begin(), v.end());
    std::ranges::rotate(v, v.begin()+1);

    std::cout << "Algorithms:\n";
    for (int x : v)
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::vector<int> sample_src{1,2,3,4,5,6,7,8,9,10};
    std::vector<int> sample_out;
    std::mt19937 gen(std::random_device{}());

    std::ranges::sample(sample_src, std::back_inserter(sample_out), 4, gen);

    std::cout << "Sample:\n";
    for (int x : sample_out)
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::vector<int> transformed;
    transform_if(sample_src,
                 std::back_inserter(transformed),
                 [](int x){ return x % 2 == 0; },
                 [](int x){ return x * x; });

    std::cout << "transform_if:\n";
    for (int x : transformed)
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::vector<double> real{3, -0.5, 2, 7};
    std::vector<double> pred{2.5, 0.0, 2, 8};

    std::cout << "MAE = " << mae(real, pred) << '\n';
    std::cout << "MSE = " << mse(real, pred) << "\n\n";

    std::cout << "views::filter:\n";
    for (int x : sample_src | std::views::filter([](int x){ return x % 2; }))
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::cout << "views::drop:\n";
    for (int x : sample_src | std::views::drop(5))
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::vector<std::vector<int>> nested{{1,2},{3,4},{5,6}};
    std::cout << "views::join:\n";
    for (int x : nested | std::views::join)
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::vector<char> letters{'a','b','c','d'};
    std::cout << "views::zip:\n";
    for (auto [n, ch] : std::views::zip(sample_src | std::views::take(4), letters))
        std::cout << '(' << n << ',' << ch << ") ";
    std::cout << "\n\n";

    std::cout << "views::stride:\n";
    for (int x : sample_src | std::views::stride(3))
        std::cout << x << ' ';
    std::cout << "\n\n";

    std::cout << "Fibonacci view:\n";
    for (auto x : Fibonacci(15))
        std::cout << x << ' ';
    std::cout << '\n';
}