#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <print>
#include <string>
#include <vector>

template < typename D = std::chrono::duration < double > > class Timer
{
public :
    Timer(std::string const & scope) : m_scope(scope) {}

    void start() {
        if (!m_running) {
            m_begin = clock_t::now();
            m_running = true;
        }
    }

    void stop() {
        if (m_running) {
            m_measurements.push_back(std::chrono::duration_cast < D > (clock_t::now() - m_begin));
            m_running = false;
        }
    }

    double average() const {
        if (m_measurements.empty()) return 0.0;
        
        double sum = 0.0;
        for (const auto& measurement : m_measurements) {
            sum += measurement.count();
        }
        return sum / m_measurements.size();
    }

    auto elapsed() const {
        return m_measurements.empty() ? D(0) : m_measurements.back();
    }

    ~Timer() {
        if (!m_measurements.empty()) {
            std::print("{} : avg={:.6f}, last={:.6f}\n", 
                      m_scope, average(), elapsed().count());
        }
    }

private :
    using clock_t = std::chrono::steady_clock;

    std::string m_scope;
    clock_t::time_point m_begin;
    bool m_running = false;
    std::vector<D> m_measurements;
};

auto calculate(std::size_t size) {
    auto x = 0.0;
    for (auto i = 0uz; i < size; ++i) {
        x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
    }
    return x;
}

auto equal(double x, double y, double epsilon = 1e-6) {
    return std::abs(x - y) < epsilon;
}

int main() {
    Timer timer("main : multiple measurements");

    for (int i = 0; i < 5; ++i) {
        timer.start();
        auto result = calculate(1'000'000);
        timer.stop();
        
        std::print("Iteration {}: {:.6f}s\n", i + 1, timer.average());
        assert(equal(result, 1'000'000));
    }
    
    std::print("Final average: {:.6f}s\n", timer.average());

    return 0;
}
