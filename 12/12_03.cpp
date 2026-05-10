#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::string_view find_longest_palindrome(std::string_view s) {
    size_t n = s.length();
    if (n == 0) return "";

    std::vector<bool> table(n * n, false);
    
    size_t start = 0;
    size_t max_len = 1;

    for (size_t i = 0; i < n; ++i) {
        table[i * n + i] = true;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        if (s[i] == s[i + 1]) {
            table[i * n + (i + 1)] = true;
            start = i;
            max_len = 2;
        }
    }

    for (size_t len = 3; len <= n; ++len) {
        for (size_t i = 0; i <= n - len; ++i) {
            size_t j = i + len - 1; 

            if (s[i] == s[j] && table[(i + 1) * n + (j - 1)]) {
                table[i * n + j] = true;
                if (len > max_len) {
                    start = i;
                    max_len = len;
                }
            }
        }
    }

    return s.substr(start, max_len);
}

int main() {
    std::string input = "babad";
    std::string_view result = find_longest_palindrome(input);
    
    std::cout << "Строка: " << input << std::endl;
    std::cout << "Наибольший палиндром: " << result << std::endl;

    std::string input2 = "cbbd";
    std::cout << "Строка: " << input2 << " -> " << find_longest_palindrome(input2) << std::endl;

    return 0;
}