#include "../../include/memory.hpp"

Memory::Memory(size_t size) : ram(size, 0) {}

uint8_t Memory::read(uint16_t address) {
    return ram[address];
}

void Memory::write(uint16_t address, uint8_t data) {
    ram[address] = data;
}

size_t Memory::get_size() {
    return ram.size();
}
