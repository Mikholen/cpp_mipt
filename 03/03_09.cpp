#include <cstdint>
#include <iostream>
#include <cassert>
#include <sstream>

class IPv4 {

public:

    IPv4() : address{0, 0, 0, 0} {};

    explicit IPv4 (std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) 
    : address{a, b, c, d} {};

    auto const operator++(int) { 
        
        auto x = *this; 
        increment (); 
        return x; 
    }

	auto const operator--(int) { 
        
        auto x = *this; 
        decrement ();
        return x; 
    }


	auto & operator++() { 
        
        increment ();
        return *this; 
    }

	auto & operator--() { 
        
        decrement (); 
        return *this; 
    }

    friend std::istream& operator>>(std::istream& stream, IPv4& ipv4) {

        int a, b, c, d;
        char dot = '.' ;
        stream >> a >> dot >> b >> dot >> c >> dot >> d;
        
        ipv4.address[0] = static_cast<std::uint8_t> (a);
        ipv4.address[1] = static_cast<std::uint8_t> (b);
        ipv4.address[2] = static_cast<std::uint8_t> (c);
        ipv4.address[3] = static_cast<std::uint8_t> (d);

        return stream;
    }

	friend auto & operator<<(std::ostream & stream, IPv4 const & ipv4) {

		return stream << static_cast<int> (ipv4.address[0]) << '.' 
                      << static_cast<int> (ipv4.address[1]) << '.' 
                      << static_cast<int> (ipv4.address[2]) << '.'
                      << static_cast<int> (ipv4.address[3]);
	}

    friend bool operator==(const IPv4& first, const IPv4& second) {

        for (int i = 0; i < 4; ++i) {

            if (first.address[i] != second.address[i])
                return false;
        }

        return true;
    }

    friend bool operator!=(const IPv4& first, const IPv4& second) {

        return !(first == second);
    }

    friend bool operator<(const IPv4& first, const IPv4& second) {

        for (int i = 0; i < 4; ++i) {

            if (first.address[i] < second.address[i]) return true;
            if (first.address[i] > second.address[i]) return false;
        }
        return false;
    }

    friend bool operator>(const IPv4& first, const IPv4& second) {

        return second < first;
    }

    friend bool operator<=(const IPv4& first, const IPv4& second) {

        return !(first > second);
    }

    friend bool operator>=(const IPv4& first, const IPv4& second) {
        
        return !(first < second);
    }

private: 

    std::uint8_t address[4]{};

    void increment() {

        for (int i = 3; i >= 0; --i) {

            if (address[i] < 255) {

                ++address[i];
                return;
            } 
            else {

                address[i] = 0;
            }
        }
    }

    void decrement() {

        for (int i = 3; i >= 0; --i) {

            if (address[i] > 0) {

                --address[i];
                return;
            } 
            else {

                address[i] = 255;
            }
        }
    }
};

void tests () {

    IPv4 adr_1{};
    IPv4 adr_2{128, 45, 77, 4};
    IPv4 adr_3{128, 45, 77, 5};
    IPv4 adr_4{12, 87, 130, 5};

    assert (adr_1 < adr_2);
    assert (++adr_2 == adr_3);
    assert (adr_3 > adr_4);
    assert (adr_1++ < adr_2);
    assert (adr_1 != adr_2);

    std::stringstream stream_1("156.78.31.0");
	std::stringstream stream_2;

    IPv4 adr_5{};
    stream_1 >> adr_5;

    stream_2 << adr_5;

    assert (stream_2.str() == stream_1.str());
}

int main () {

    tests ();
    return 0;
}