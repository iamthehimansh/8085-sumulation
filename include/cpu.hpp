#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
// Add memory include and member
#include "memory.hpp"
#include <iostream>
class CPU {
private:
    // Registers
    uint8_t A;     // Accumulator
    uint8_t B, C;  // B-C Register pair
    uint8_t D, E;  // D-E Register pair
    uint8_t H, L;  // H-L Register pair
    
    // Special registers
    uint16_t SP;   // Stack Pointer
    
    // Flag register
    struct {
        uint8_t S : 1;  // Sign flag
        uint8_t Z : 1;  // Zero flag
        uint8_t AC : 1; // Auxiliary Carry flag
        uint8_t P : 1;  // Parity flag
        uint8_t CY : 1; // Carry flag
    } flags;
    void update_flags(uint8_t value);
    uint8_t serial_in;
    uint8_t serial_out;
    bool pending_intr5;
    bool pending_intr6; 
    bool pending_intr7;
    bool mask_intr5;
    bool mask_intr6;
    bool mask_intr7;
    bool interrupts_enabled;
public:
    CPU(Memory& mem);  // Updated constructor
    Memory& memory;  // Reference to memory
    // Special registers
    uint16_t PC;   // Program Counter
    uint8_t Halted:1;
    // uint8_t interrupts_enabled:1;
    void reset();
    void execute_instruction(uint8_t opcode, uint8_t operand=0, uint8_t operand2 = 0);
    void print_registers();
    void print_memory(uint16_t start_address, uint16_t end_address);

    
};

#endif // CPU_HPP