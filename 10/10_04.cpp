#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>

namespace ublas = boost::numeric::ublas;

using ull = unsigned long long int;
using Matrix = ublas::matrix<ull>;

Matrix multiply(const Matrix &a, const Matrix &b)
{
    Matrix r(2, 2);

    r(0, 0) = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0);
    r(0, 1) = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1);
    r(1, 0) = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0);
    r(1, 1) = a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1);

    return r;
}

Matrix identity()
{
    Matrix I(2, 2);

    I(0, 0) = 1; I(0, 1) = 0;
    I(1, 0) = 0; I(1, 1) = 1;

    return I;
}

Matrix power(Matrix base, ull n)
{
    Matrix result = identity();

    while (n > 0)
    {
        if (n & 1)
            result = multiply(result, base);

        base = multiply(base, base);
        n >>= 1;
    }

    return result;
}

ull fibonacci(ull n)
{
    if (n == 0) return 0;

    Matrix M(2, 2);
    M(0, 0) = 1; M(0, 1) = 1;
    M(1, 0) = 1; M(1, 1) = 0;

    Matrix R = power(M, n - 1);

    return R(0, 0);
}

int main()
{
    ull n;
    std::cin >> n;

    std::cout << fibonacci(n) << "\n";
}