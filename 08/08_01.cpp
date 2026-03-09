#include <iostream>

class Entity_v1 {
private:
    int secret_value = 42;
    
public:
    Entity_v1(int value = 42) : secret_value(value) {}
    
    int get_secret() const { return secret_value; }
};

class Entity_v2 {
public:
    int public_value = 0;
};

void hack_method1(Entity_v1& victim) {
    Entity_v2& fake = reinterpret_cast<Entity_v2&>(victim);
    fake.public_value = 999;
    std::cout << "Method 1: hacked via reinterpret_cast: " 
              << victim.get_secret() << std::endl;
}

class SafeContainer {
private:
    int protected_data = 100;
    
public:
    int read_data() const { return protected_data; }
};

void hack_method2(SafeContainer& target) {
    int* raw_ptr = reinterpret_cast<int*>(&target);
    *raw_ptr = 777;
    std::cout << "Method 2: hacked via raw pointer cast: " 
              << target.read_data() << std::endl;
}

int main() {
    Entity_v1 obj1(42);
    std::cout << "Original: " << obj1.get_secret() << std::endl;
    
    hack_method1(obj1);
    
    SafeContainer obj2;
    std::cout << "\nContainer original: " << obj2.read_data() << std::endl;
    
    hack_method2(obj2);
    
    return 0;
}
