#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "cpu.hpp"
#include "memory.hpp"

class Simulator {
private:
    Memory memory;
    CPU cpu;

public:
    Simulator();
    void load_program(const std::vector<uint8_t>& program, uint16_t start_address = 0);
    void run();
    void step();
    void reset();
    void print_registers();
    void print_memory(uint16_t start, uint16_t end);
};

#endif // SIMULATOR_HPP