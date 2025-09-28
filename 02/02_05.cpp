#include <iostream>
#include <cmath>

int main () {

    double epsilon = NAN;
    double next_summand = 1.0, exponent = 0.0;
    double i = 1.0;

    std::cin >> epsilon;

    while (next_summand >= epsilon) {

        exponent += next_summand;
        next_summand /= i++;
    }

    std::cout << exponent << '\n';

    return 0;
}