#include <iostream>

using Func = void(*)();

void test_impl()
{
    std::cout << "Hello from test\n";
}

class Wrapper
{
public:
    Wrapper(Func f = nullptr) : m_func(f) {}

    Func operator*() const
    {
        return m_func;
    }

    operator Func() const
    {
        return m_func;
    }

private:
    Func m_func;
};

Wrapper test()
{
    return Wrapper(test_impl);
}

int main()
{
    Wrapper function = test();

    (*function)();
}