#include <iostream>
#include <numeric>
#include <cassert>

int get_gcd (int a, int b) {

    if (b == 0)
        return a;
    
    else
        return get_gcd (b, a % b);
}

int get_lcm (int a, int b) {

    return a * b / get_gcd (a, b);
}

void tests() {

    assert (get_gcd(0, 0) == 0);
    assert (get_gcd(0, 5) == 5);
    assert (get_gcd(5, 0) == 5);
    assert (get_gcd(12, 18) == 6);
    assert (get_gcd(100, 10) == 10);
    assert (get_gcd(17, 13) == 1);
    assert (get_gcd(-27, 9) == 9);

    assert (get_lcm(0, 5) == 0);
    assert (get_lcm(5, 0) == 0);
    assert (get_lcm(4, 6) == 12);
    assert (get_lcm(10, 15) == 30);
    assert (get_lcm(7, 3) == 21);

    int tests[][2] = {

        {12, 18},
        {100, 10},
        {7, 13},
        {0, 5},
        {5, 0},
        {27, 9}
    };

    for(auto &pair : tests) {

        int a = pair[0], b = pair[1];

        assert (get_gcd(a, b) == std::gcd(a, b));
        assert (get_lcm(a, b) == std::lcm(a, b));
    }
}

int main () {

    // int a, b;

    // std::cin >> a >> b;

    // assert ((get_gcd (a, b) == std::gcd (a, b)) && (get_lcm (a, b) == std::lcm (a, b)));

    // std::cout << "GCD : " << get_gcd (a, b) << '\n';
    // std::cout << "LCM : " << get_lcm (a, b) << '\n'; 

    tests ();

    return 0;
}