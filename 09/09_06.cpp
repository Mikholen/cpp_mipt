#include <iostream>
#include <iterator>


class FibonacciRange {

public:

    class Iterator {

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const int*;
        using reference         = const int&;


        Iterator(int count = 0, int limit = 0)
            : m_a(0), m_b(1), m_count(count), m_limit(limit) {}


        Iterator& operator++() {

            int next = m_a + m_b;
            m_a = m_b;
            m_b = next;

            ++m_count;
            return *this;
        }

        Iterator operator++(int) {

            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }


        int operator*() const {

            return m_a;
        }


        bool operator==(const Iterator& other) const {

            return m_count == other.m_count;
        }

        bool operator!=(const Iterator& other) const {

            return !(*this == other);
        }

    private:

        int m_a;
        int m_b;
        int m_count;
        int m_limit;
    };


    FibonacciRange(int n) : m_n(n) {}

    Iterator begin() const { return Iterator(0, m_n); }
    Iterator end()   const { return Iterator(m_n, m_n); }

private:

    int m_n;
};


int main() {

    FibonacciRange fib(10);

    for (auto x : fib) {

        std::cout << x << " ";
    }
}