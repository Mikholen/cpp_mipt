#include <cassert>
#include <sstream>
#include <string>

using namespace std::literals;
#include "08.13.hpp"

int sign(const Integer& x) {
    std::stringstream ss;
    ss << x;
    std::string s = ss.str();
    if (s == "0") return 0;
    return s[0] == '-' ? -1 : 1;
}

Integer abs(Integer x) {
    Integer zero("0");
    if (sign(x) < 0) {
        x = zero - x;
    }
    return x;
}

Integer& operator%=(Integer& x, const Integer& y) {
    Integer quotient = x / y;
    x = x - quotient * y;
    return x;
}

Integer operator%(const Integer& x, const Integer& y) {
    Integer quotient = x / y;
    return x - quotient * y;
}

Integer pow(Integer base, unsigned int exponent) {
    Integer result("1");
    while (exponent > 0) {
        if (exponent % 2 == 1) result = result * base;
        base = base * base;
        exponent /= 2;
    }
    return result;
}

int main() {
    Integer x = std::string(32, '1');
    Integer y = std::string(32, '2');
    Integer x_orig = x;
    Integer y_orig = y;

    assert((x += y) == "+33333333333333333333333333333333"s);
    assert((x -= y) == "+11111111111111111111111111111111"s);
    assert((x *= y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x /= y) == "+11111111111111111111111111111111"s);

    x = x_orig;
    y = y_orig;

    assert((x++) == "+11111111111111111111111111111111"s);
    assert((x--) == "+11111111111111111111111111111112"s);
    assert((++y) == "+22222222222222222222222222222223"s);
    assert((--y) == "+22222222222222222222222222222222"s);

    x = x_orig;
    y = y_orig;

    assert((x + y) == "+33333333333333333333333333333333"s);
    assert((x - y) == "-11111111111111111111111111111111"s);
    assert((x * y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x / y) == "+0"s);

    assert((x < y) == 1);
    assert((x > y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 0);
    assert((x == y) == 0);
    assert((x != y) == 1);

    std::stringstream stream_1(std::string(32, '1'));
    std::stringstream stream_2;
    stream_1 >> x;
    stream_2 << x;
    assert(stream_2.str() == std::string(32, '1'));

    x = x_orig;
    assert(sqrt(multiply(x, x)) == x);

    Integer a("12345678901234567890");
    Integer b("987654321");
    Integer zero("0");
    
    assert(sign(a) == 1);
    assert(sign(zero - a) == -1);
    assert(sign(zero) == 0);
    
    assert(abs(zero - a) == a);
    
    assert(pow(Integer("2"), 10) == "1024"s);
    assert(pow(Integer("3"), 5) == "243"s);
    assert(pow(Integer("10"), 0) == "1"s);

    return 0;
}
