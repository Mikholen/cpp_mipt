#include <iostream>
#include <string>
#include <dlfcn.h>

typedef void (*TestFunc)(int);

int main() {
    std::cout << "\nAvailable:\n";
    std::cout << "  libtest_v1.so  (x2)\n";
    std::cout << "  libtest_v2.so  (square)\n\n";
    
    std::string lib_name;
    std::cout << "Enter lib name: ";
    std::cin >> lib_name;
    
    std::cout << "Loading: " << lib_name << "\n";
    
    void* handle = dlopen(("./" + lib_name).c_str(), RTLD_LAZY);
    if (!handle) {
        handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    }
    
    if (!handle) {
        std::cout << "dlopen failed: " << dlerror() << "\n";
        return 1;
    }
    
    std::cout << "dlopen success\n";
    
    TestFunc test = (TestFunc)dlsym(handle, "test");
    if (!test) {
        std::cout << "dlsym failed: " << dlerror() << "\n";
        dlclose(handle);
        return 1;
    }
    
    std::cout << "\nEnter number:\n";
    float num = 0.0;
    std::cin >> num;
    test(num);
    dlclose(handle);
    
    std::cout << "\nDone\n";
    return 0;
}
