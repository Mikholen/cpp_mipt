#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <iostream>

template <typename T>
class Vector {

public :

	Vector () : m_array (nullptr), m_size (0) {

		std::cout << "Vector:: Vector (1)" << '\n';
	}

	Vector (std::initializer_list < T > list) : m_size (std::size (list)) {

		std::cout << "Vector:: Vector (2)" << '\n';

		m_array = m_size ? new T[m_size]{} : nullptr;

		std::ranges::copy (list, m_array);
	}

	Vector (Vector const & other) : m_size (other.m_size) {

		std::cout << "Vector:: Vector (3)" << '\n';

		m_array = m_size ? new T[m_size]{} : nullptr;

		std::ranges::copy (other.m_array, other.m_array + other.m_size, m_array);
	}

	Vector (Vector && other)
	: 
		m_array (std::exchange(other.m_array, nullptr)), 
		
		m_size  (std::exchange(other.m_size,  0      )) {

		std::cout << "Vector:: Vector (4)" << '\n';
	}

   ~Vector () {

		std::cout << "Vector::~Vector" << '\n';

		delete[] m_array;
	}

	auto & operator=(Vector other) {

		std::cout << "Vector::operator= (4)" << '\n';

		swap (other);

		return *this;
	}

	void swap (Vector & other) {

		std::swap (m_array, other.m_array);
		
		std::swap (m_size,  other.m_size );
	}

    std::size_t size () {

        return m_size;
    }

    std::size_t capacity () {

        return m_capacity;
    }

    void push_back (const T element) {

        if (m_size + 1 > m_capacity) {

            m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            auto new_arr = new T[m_capacity]{};
            std::ranges::copy (m_array, m_array + m_size, new_arr);

            delete[] m_array;
            m_array = new_arr;
            m_array[m_size] = element;

            m_size++;
        }

        else {

            m_array[m_size] = element;
            m_size++;
        }
    }

    bool empty () {

        return (m_size == 0);
    }

    void clear () {

        for (std::size_t i = 0; i < m_size; i++) {

            m_array[i] = 0;
        }

        m_size = 0;
    }

private :

	T * m_array = nullptr;

	std::size_t m_size     = 0;
    std::size_t m_capacity = 0;
};

void tests () {

    Vector<int> vector_1;

	Vector vector_2 = { 1, 2, 3, 4, 5 };

	Vector vector_3 = vector_2;

	Vector vector_4 = std::move (vector_3);

	vector_3 = vector_2;

	vector_4 = std::move (vector_3);

}

int main () {

	tests ();
}