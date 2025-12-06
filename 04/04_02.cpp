#include <cassert>
#include <vector>
#include <limits>

template <typename ... Ts> 
double sum_ (Ts ... xs) {

    return (0 + ... + xs);
}

template <typename ... Ts> 
double avg_ (Ts ... xs) {

    return (0 + ... + xs) / sizeof... (xs);
}

template <typename ... Ts>
double max_ (Ts ... xs) {

    auto m = std::numeric_limits<double>::lowest ();
    ((m = std::max (m, xs)), ...);
    return m;
}

template <typename ... Ts>
double min_ (Ts ... xs) {

    auto m = std::numeric_limits<double>::max ();
    ((m = std::min (m, xs)), ...);
    return m;
}

bool isclose (double a, double b) {

    return a - b < 0.0001;
}

int main () {

    assert ( isclose (5.6, sum_ (3.2, 6.7, 1.1, -5.4)));
    assert ( isclose (13.2, max_ (13.2, 6.7, 1.1, -5.4)));
    assert ( isclose (-92.5, min_ (3.2, -92.5, 0.0, 6.7, 1.1, -5.4)));
    assert ( isclose (1.4, avg_ (3.2, 6.7, 1.1, -5.4)));

    return 0;
}