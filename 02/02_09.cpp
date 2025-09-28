#include <iostream>
#include <numeric>
#include <cassert>

int get_gcd (int a, int b);
int get_lcm (int a, int b);

int main () {

    int a, b;

    std::cin >> a >> b;

    assert((get_gcd (a, b) == std::gcd (a, b)) && (get_lcm (a, b) == std::lcm (a, b)));

    std::cout << "GCD : " << get_gcd (a, b) << '\n';
    std::cout << "LCM : " << get_lcm (a, b) << '\n'; 

    return 0;
}

int get_gcd (int a, int b) {

    if (b == 0)
        return a;
    
    else
        return get_gcd (b, a % b);
}

int get_lcm (int a, int b) {

    return a * b / get_gcd (a, b);
}