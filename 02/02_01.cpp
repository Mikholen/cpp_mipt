#include <iostream>
#include <cmath>

const double phi = (1 + std::sqrt (5)) / 2;

int fibonacci_number_calculator (int N);
int fibonacci_number_calculator_eval (int N);

int main () {

    int N = 0;

    std::cin >> N;

    // How to evaluate precision of formula
    // for (int i = 1; i <= N; i++) {
    //     std::cout << fibonacci_number_calculator(i) << ' ';
    // }

    // std::cout << '\n';

    std::cout << fibonacci_number_calculator (N) << '\n';
    std::cout << "Correct: " << fibonacci_number_calculator_eval (N) << '\n';

    return 0;
}

int fibonacci_number_calculator (int N) {

    double number = NAN;

    number = (std::pow (phi, N) - std::pow (-phi, -N)) / (2 * phi - 1);

    return static_cast<int>(number);
}

int fibonacci_number_calculator_eval (int N) {

    if ((N == 1) || (N == 2)) {

        return 1;
    }

    else {

        return fibonacci_number_calculator_eval (N - 2) + fibonacci_number_calculator_eval (N - 1);
    }
}
