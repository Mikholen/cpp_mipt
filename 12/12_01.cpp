#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

void run_test(const std::string& input_rub, long double rate) {
    const char* ru_loc = "ru_RU.UTF-8";
    const char* us_loc = "en_US.UTF-8";

    long double units_rub;
    std::stringstream ss_in;
    ss_in.imbue(std::locale(ru_loc));
    ss_in << input_rub;
    ss_in >> std::get_money(units_rub);

    if (ss_in.fail()) {
        std::cout << "Failed to parse: " << input_rub << std::endl;
        return;
    }

    long double units_usd = units_rub / rate;

    std::stringstream ss_out;
    ss_out.imbue(std::locale(us_loc));
    ss_out << std::showbase << std::put_money(units_usd);

    std::cout << std::left << std::setw(20) << input_rub 
              << " -> " << ss_out.str() << std::endl;
}

int main() {
    const long double rate = 91.45;
    
    std::vector<std::string> test_cases = {
        "100,00 руб.",
        "0,50 руб.",
        "1000000,00 руб.",
        "54,25 руб.",
        "0,00 руб.",
        "1234567,89 руб."
    };

    std::cout << "Exchange Rate: 1 USD = " << rate << " RUB" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    for (const auto& test : test_cases) {
        run_test(test, rate);
    }

    return 0;
}