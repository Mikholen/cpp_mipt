#include <limits>

template <int X> struct Fibonacci {

    int const n = X;
    static inline int const value = Fibonacci < X - 1 > ::value + Fibonacci < X - 2 > ::value; 
    static_assert (value >= std::numeric_limits<int>::min() && value <= std::numeric_limits<int>::max());
};

template <>      struct Fibonacci <1> {

    int const n = 1;
    static inline int const value = 1;
    static_assert (value >= std::numeric_limits<int>::min() && value <= std::numeric_limits<int>::max());
};

template <>      struct Fibonacci <2> {

    int const n = 2;
    static inline int const value = 1;
    static_assert (value >= std::numeric_limits<int>::min() && value <= std::numeric_limits<int>::max());
};

template <int X>
auto const F_val = Fibonacci <X> ::value;

int main () {

    static_assert (F_val <3> == 2);
    static_assert (F_val <5> == 5);
    static_assert (F_val <7> == 13);

    return 0;
}