#include <iostream>
#include <cmath>
#include <cassert>
#include <unordered_set>

std::unordered_set<int> punctuation_marks_codes = {'!', '(', ')', ',', '.', ':', ';', '?'};

int main () {

    char symbol;

    std::cin >> symbol;

    int symbol_code = symbol, num_case = -8;

    assert (symbol_code >= 32 && symbol_code <= 127);

    num_case = (symbol_code >= '0' && symbol_code <= '9') * 1 +
               (symbol_code >= 'A' && symbol_code <= 'Z') * 2 +
               (symbol_code >= 'a' && symbol_code <= 'z') * 3 +
               (punctuation_marks_codes.count (symbol_code)) * 4;

    switch (num_case) {

        case 0:
            std::cout << "Other symbol" << '\n';
            return 0;

        case 1:
            std::cout << "Decimal number" << '\n';
            return 0;

        case 2:
            std::cout << "Capital letter" << '\n';
            return 0;

        case 3:
            std::cout << "Lowercase letter" << '\n';
            return 0;

        case 4:
            std::cout << "Punctuation mark" << '\n';
            return 0;

    }
}