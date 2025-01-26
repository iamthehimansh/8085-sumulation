#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <vector>

class Memory {
private:
    std::vector<uint8_t> ram;

public:
    Memory(size_t size = 65536);  // Default 64K memory
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
    size_t get_size();
};

#endif // MEMORY_HPP