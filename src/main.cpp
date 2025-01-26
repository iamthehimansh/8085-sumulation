#include "../include/simulator.hpp"
#include "../include/instructions.hpp"
#include <iostream>

int main() {
    Simulator sim;
    
    std::vector<uint8_t> program = {
        // Store the number in memory location 0x2000
        MVI_A, 0x05,    // Number to print (5 in this example)
        STA, 0x00, 0x20,
        
        // Main loop
        LDA, 0x00, 0x20,    // Load counter
        MOV_B_A,           // Store counter in B
        
        LDA, 0x00, 0x20,    // Load number to print
        SIM,                // Output to serial
        DCR_B,              // Decrement counter
        MOV_A_B,
        CPI, 0x00,         // Compare with 0
        JNZ, 0x0c, 0x00,   // Jump to loop_start if not zero
        
        HLT                 // Halt the program
    };
    
    sim.load_program(program);
    sim.run();
    sim.print_registers();
    
    return 0;
}