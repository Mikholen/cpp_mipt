#include <cstdint>
#include <cassert>

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
        return -126 + shift;
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
    
    return 0;
}
