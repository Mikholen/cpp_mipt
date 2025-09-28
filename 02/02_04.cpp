#include <iostream>

int get_number_power (int number, int power);

int main () {

    std::cout << "Armstrong numbers : ";

    for (int i = 1; i <= 9; i++) {

        for (int j = 0; j <= 9; j++) {

            for (int k = 0; k <= 9; k++) {

            if (get_number_power (i, 3) + get_number_power (j, 3) +
                get_number_power (k, 3) == i * 100 + j * 10 + k) {

                std::cout << i * 100 + j * 10 + k << ' ';
            }
            }
        }
    }

    std::cout << '\n';

    return 0;
}

int get_number_power (int number, int power) {

    int result = 1;
    
    for (int i = 0; i < power; i++) {

        result *= number;
    }

    return result;
}