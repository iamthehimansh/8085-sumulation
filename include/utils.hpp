#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>

namespace Utils {
    // Flag calculation functions
    bool calculate_parity(uint8_t value);
    bool calculate_zero(uint8_t value);
    bool calculate_sign(uint8_t value);
    
    // Helper functions
    uint16_t combine_bytes(uint8_t high, uint8_t low);
    void split_word(uint16_t word, uint8_t& high, uint8_t& low);
}

#endif // UTILS_HPP