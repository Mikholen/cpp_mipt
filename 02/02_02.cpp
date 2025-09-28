#include <iostream>
#include <cmath>

const double epsilon = 1e-8;

bool is_close (double number_1, double number_2);

int main () {

    double a = NAN, b = NAN, c = NAN;

    std::cin >> a >> b >> c;

    if (is_close (a, 0.0)) {

        if (is_close (b, 0.0) && is_close (c, 0.0)) {

            std::cout << "Any number is a solution" << '\n';
            return 0;
        }

        if (is_close (b, 0.0)) {

            std::cout << "Equation has no solutions" << '\n';
            return 0;
        }

        else {

            std::cout << "x = " << -c / b << '\n';
            return 0;
        }
    }

    double discriminant = std::pow (b, 2) - 4 * a * c;

    if (discriminant > 0) {
        
        std::cout << "x1 = " << (-b + std::sqrt (discriminant)) / (2 * a)
                  << "\nx2 = " << (-b - std::sqrt (discriminant)) / (2 * a) << '\n';
        return 0;
    }

    else if (discriminant < 0) {
        
        std::cout << "No real solutions (discriminant < 0)" << '\n';
        return 0;
    }

    else {

        std::cout << "x = " << -b / (2 * a) << '\n';
        return 0;
    }

}

bool is_close (double number_1, double number_2) {

    if (std::abs (number_1 - number_2) < epsilon) {
        return true;
    }

    return false;
}