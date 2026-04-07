#include <iostream>
#include <string>
#include <source_location>

class Tracer {

public:
    
    Tracer(const std::string& func_name = "",
           const std::source_location& loc = std::source_location::current())
        : m_func(func_name.empty() ? loc.function_name() : func_name),
          m_location(loc) {

        std::cout << "[ENTER] "
                  << m_func
                  << " (" << m_location.file_name()
                  << ":" << m_location.line() << ")\n";
    }

    ~Tracer() {

        std::cout << "[EXIT ] "
                  << m_func
                  << " (" << m_location.file_name()
                  << ":" << m_location.line() << ")\n";
    }

private:
    std::string m_func;
    std::source_location m_location;
};

#ifndef NDEBUG
    #define trace() Tracer tracer_instance
#else
    #define trace() ((void)0)
#endif

void foo()
{
    trace();
    std::cout << "Inside foo\n";
}

void bar()
{
    trace();
    foo();
}

int main() {
    
    trace();
    bar();

    return 0;
}