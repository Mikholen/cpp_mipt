#include <iostream>
#include <cmath>
#include <optional>
#include <variant>
#include <utility>
#include <iomanip>

const double epsilon = 1e-8;

bool is_close(double number_1, double number_2) {
    return std::abs(number_1 - number_2) < epsilon;
}

using Roots = std::variant<double, std::pair<double, double>, std::monostate>;


std::optional<Roots> solve(double a, double b, double c) {
    if (is_close(a, 0.0)) {

        if (is_close(b, 0.0)) {
            if (is_close(c, 0.0)) {
                return Roots{std::monostate{}}; 
            }
            return std::nullopt;  
        }

        return Roots{double(-c / b)};
    }
    
    double discriminant = std::pow(b, 2) - 4 * a * c;
    
    if (discriminant > epsilon) {
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-b + sqrt_d) / (2 * a);
        double x2 = (-b - sqrt_d) / (2 * a);
        return Roots{std::make_pair(x1, x2)};
    }
    
    if (discriminant < -epsilon) {
        return std::nullopt; 
    }
    
    return Roots{double(-b / (2 * a))};
}

int main() {
    double a = NAN, b = NAN, c = NAN;
    std::cin >> a >> b >> c;
    
    auto result = solve(a, b, c);
    
    if (!result) {
        std::cout << "Equation has no solutions\n";
        return 0;
    }
    
    auto& roots = *result;
    
    if (std::get_if<std::monostate>(&roots)) {
        std::cout << "Any number is a solution\n";
    }
    else if (auto* single = std::get_if<double>(&roots)) {
        std::cout << std::fixed << std::setprecision(10);
        std::cout << "x = " << *single << '\n';
    }
    else if (auto* pair = std::get_if<std::pair<double, double>>(&roots)) {
        std::cout << std::fixed << std::setprecision(10);
        std::cout << "x1 = " << pair->first << "\nx2 = " << pair->second << '\n';
    }
    
    return 0;
}
