#include <cstdint>
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>

int floor_log2_int(int n) {
    if (n <= 0) return -1;
    
    unsigned int u = static_cast<unsigned int>(n);
    int result = 0;
    
    while (u > 1) {
        u >>= 1;
        ++result;
    }
    
    return result;
}

union FloatBits {
    float f;
    unsigned int u;
};

int floor_log2_float(float x) {
    if (x <= 0.0f) return -1;
    
    FloatBits fb;
    fb.f = x;
    unsigned int bits = fb.u;
    
    int exponent = (bits >> 23) & 0xFF;
    
    if (exponent == 0xFF) {
        return -1;
    }
    
    if (exponent == 0) {
        unsigned int mantissa = bits & 0x7FFFFF;
        if (mantissa == 0) return -1;
        
        int shift = 0;
        while (mantissa > 1) {
            mantissa >>= 1;
            ++shift;
        }
        return -126 + shift; // 126 is not correct, 149 is 
    }
    
    return exponent - 127;
}

int main() {
    assert(floor_log2_int(1) == 0);
    assert(floor_log2_int(2) == 1);
    assert(floor_log2_int(3) == 1);
    assert(floor_log2_int(4) == 2);
    assert(floor_log2_int(7) == 2);
    
    assert(floor_log2_int(0) == -1);
    assert(floor_log2_int(-1) == -1);
    
    assert(floor_log2_float(1.0f) == 0);
    assert(floor_log2_float(2.0f) == 1);
    assert(floor_log2_float(3.0f) == 1);
    assert(floor_log2_float(4.0f) == 2);
    
    assert(floor_log2_float(0.5f) == -1);
    assert(floor_log2_float(0.25f) == -2);
    
    assert(floor_log2_float(1.0f / 128.0f) == -7);
        
    assert(floor_log2_float(0.0f) == -1);
    assert(floor_log2_float(-1.0f) == -1);
    
    
	std::vector<double> vector_double
    {
		3.145293e-40, // Failed
		1e-44, // Failed
		7.1746481e-43, // Failed
		3.145293e-25,
		3.145293e-2,
		3.145293e2,
		3.145293e4,
		3.145293e35,
		5.4210109e-20,
		0.25,
		1,
		1024,
		1.8446744e+19
	};
	
	std::vector<float> vector_float;
	for(const auto x_double: vector_double)
	{
		vector_float.push_back(x_double);
	}
	
	for(const auto x_float: vector_float)
	{
		const int answer_custom = floor_log2_float(x_float);
		const int answer_std = std::floor(std::log2(x_float));
		
		std::cout << (answer_custom == answer_std ? "passed" : "failed") << " " << x_float << std::endl;
		std::cout << "custom = " << answer_custom << ", std-ans = " << answer_std << "\n" << std::endl;
	}
	
    return 0;
}

/*
 * Score is 6/10
 * Subnormal case not working properly 
 * Did not include test cases to check subnormal cases
 */
