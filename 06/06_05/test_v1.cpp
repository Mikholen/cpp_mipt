#include <iostream>

extern "C" {
    void test(int value) {
        std::cout << "Insert number: " << value << '\n';
        std::cout << "num x2: " << (value * 2) << "\n";
    }
}
