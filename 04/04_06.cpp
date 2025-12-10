#include <type_traits>
#include <array>
#include <cmath>

consteval auto exponent (double const epsilon) {

    double exp = 0.0;
    for (double next_summand = 1.0, i = 1.0;
         next_summand >= epsilon;
         exp += next_summand, next_summand /= i++);
    
    return exp;
}

consteval auto pi (double const epsilon) {

    double calc_pi = 1.0;
    for (double n = 3.0, sign = -1.0;
         1 / n >= epsilon;
         calc_pi += sign / n, n += 2, sign *= -1.0);
    
    return 4 * calc_pi;
}

int main () {

    constexpr std::array eps = {1e-2, 1e-4, 1e-6, 1e-8};

    static_assert(exponent(eps[0]) > 2.7);
    static_assert(exponent(eps[1]) > 2.718);
    static_assert(exponent(eps[2]) > 2.71828);

    static_assert(pi(eps[1]) > 3.141);
    static_assert(pi(eps[0]) > 3.1);

    return 0;
}