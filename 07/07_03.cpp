#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>

namespace hr {
namespace model {

enum class Status { ok, error };

class Person {
public:
    Person(std::string name, std::int32_t lvl, double income, std::int64_t num)
        : m_name(std::move(name)), m_level(lvl), m_income(income), m_number(num) {}
    
    std::string const& get_name() const noexcept { return m_name; }
    std::int32_t get_level() const noexcept { return m_level; }
    double get_income() const noexcept { return m_income; }
    std::int64_t get_number() const noexcept { return m_number; }

private:
    std::string m_name;
    std::int32_t m_level = 0;
    double m_income = 0.0;
    std::int64_t m_number = 0;
};

} 

namespace service {

void persist(model::Status outcome, std::int64_t emp_num) {
    std::cout << "persist: " 
              << (outcome == model::Status::ok ? "ok" : "error")
              << " emp=" << emp_num << "\n";
}

void process(const model::Person& emp) {
    std::cout << "process: " << emp.get_name() << "\n";
    
    if (emp.get_level() == 10 || emp.get_income() > 1000000.0) {
        persist(model::Status::ok, emp.get_number());
    } else {
        persist(model::Status::error, emp.get_number());
    }
}

} 
} 

int main() {
    try {
        hr::model::Person top("Carol", 10, 800000.0, 101);
        hr::model::Person high("David", 7, 1500000.0, 102);
        hr::model::Person regular("Elena", 4, 75000.0, 103);
        
        hr::service::process(top);
        hr::service::process(high);
        hr::service::process(regular);
    } catch (const std::exception& ex) {
        std::cerr << "exception: " << ex.what() << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
    
    return 0;
}

// 1 element of normal branching (if-else), 6 potential exceptions from Person method calls and save()
