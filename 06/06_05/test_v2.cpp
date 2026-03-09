#include <iostream>

extern "C" {
    void test(int value) {
        std::cout << "Insert number: " << value << '\n';
        std::cout << "num **2: " << (value * value) << "\n";
    }
}
