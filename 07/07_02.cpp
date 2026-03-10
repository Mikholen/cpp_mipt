#include <boost/operators.hpp>
#include <cassert>
#include <cmath>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>
#include <compare>
#include <stdexcept>
#include <variant>
#include <optional>
#include <iostream>

class Exception : public std::exception {
public:
    explicit Exception(const std::string& message) : m_message(message) {}
    
    const char* what() const noexcept override {
        return m_message.c_str();
    }
    
private:
    std::string m_message;
};

template <typename T>
class Rational : public boost::addable<Rational<T>>,
                        boost::subtractable<Rational<T>>,
                        boost::multipliable<Rational<T>>,
                        boost::dividable<Rational<T>>,
                        boost::incrementable<Rational<T>>,
                        boost::decrementable<Rational<T>>
{
public:
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den) {
        if (m_den == 0) {
            throw Exception("Denominator cannot be zero");
        }
        reduce();
    }

    explicit operator double() const { 
        return 1.0 * m_num / m_den;
    }

    auto & operator+=(Rational const & other) {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }

    auto & operator-=(Rational const & other) { 
        return *this += Rational(other.m_num * -1, other.m_den);
    }

    auto & operator*=(Rational const & other) {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }
    
    auto & operator/=(Rational const & other) { 
        return *this *= Rational(other.m_den, other.m_num);
    }

    auto & operator++() { *this += 1; return *this; }
    auto & operator--() { *this -= 1; return *this; }

    friend std::strong_ordering operator<=>(const Rational& lhs, const Rational& rhs) {
        T lhs_cross = lhs.m_num * rhs.m_den;
        T rhs_cross = rhs.m_num * lhs.m_den;
        if (lhs_cross < rhs_cross) return std::strong_ordering::less;
        else if (lhs_cross > rhs_cross) return std::strong_ordering::greater;
        else return std::strong_ordering::equal;
    }

    friend bool operator==(const Rational& lhs, const Rational& rhs) {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend auto & operator>>(std::istream & stream, Rational & rational) {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend auto & operator<<(std::ostream & stream, Rational const & rational) {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    void reduce() {
        if (m_den < 0) {
            m_num = -m_num;
            m_den = -m_den;
        }
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }

    T m_num = 0, m_den = 1;
};

auto equal(double x, double y, double epsilon = 1e-6) {
    return std::abs(x - y) < epsilon;
}

void demonstrate_std_exceptions() {
    std::cerr << "\n=== Standard exceptions ===\n";
    
    try {
        std::vector<int> vec = {1, 2, 3};
        (void)vec.at(5);
    } catch (const std::out_of_range& e) {
        std::cerr << "std::out_of_range: " << e.what() << "\n";
    }
    
    try {
        std::optional<int> opt;
        opt.value();  // Генерирует bad_optional_access
    } catch (const std::bad_optional_access& e) {
        std::cerr << "std::bad_optional_access: " << e.what() << "\n";
    }
    
    try {
        std::variant<int, double> v = 42;
        (void)std::get<double>(v);
    } catch (const std::bad_variant_access& e) {
        std::cerr << "std::bad_variant_access: " << e.what() << "\n";
    }
    
    /*
     * Missing
     * std::bad_alloc,
     * std::length_error
     * std::out_of_range
     */
}


int main() {
    try {
        [[maybe_unused]] Rational<int> x = 1;
        Rational<int> y(2, 0);
    } catch (const Exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "Unknown exception\n";
    }
    
    try {
        Rational<int> x(1, 2), y(1, 1);
        assert(equal(static_cast<double>(x), 0.5));
        assert((x += y) == Rational<int>(3, 2));
        assert((x *= 2) == Rational<int>(3, 1));
        assert((x /= 2) == Rational<int>(3, 2));
        assert((x + 1) == Rational<int>(5, 2));
        assert((++x) == Rational<int>(5, 2));
    } catch (const std::exception& e) {
        std::cerr << "Rational test error: " << e.what() << "\n";
    }
    
    demonstrate_std_exceptions();
    // Add general catch (...) here too
    return 0;
}
