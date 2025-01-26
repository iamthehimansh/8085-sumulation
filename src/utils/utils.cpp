#include "../../include/utils.hpp"

namespace Utils {
    bool calculate_parity(uint8_t value) {
        uint8_t count = 0;
        for (int i = 0; i < 8; i++) {
            if (value & (1 << i)) count++;
        }
        return (count % 2) == 0;
    }

    bool calculate_zero(uint8_t value) {
        return value == 0;
    }

    bool calculate_sign(uint8_t value) {
        return (value & 0x80) != 0;
    }

    uint16_t combine_bytes(uint8_t high, uint8_t low) {
        return (static_cast<uint16_t>(high) << 8) | low;
    }

    void split_word(uint16_t word, uint8_t& high, uint8_t& low) {
        high = (word >> 8) & 0xFF;
        low = word & 0xFF;
    }
}