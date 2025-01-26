#include "../include/simulator.hpp"
#include "../include/instructions.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

int main() {
    Simulator sim;
    std::vector<uint8_t> program;
    
    std::cout << "8085 Simulator by iamthehimansh\n";
    std::cout << "1. Load from binary file\n";
    std::cout << "2. Input instructions manually\n";
    std::cout << "3. Input instructions using mnemonics\n";
    std::cout << "Enter choice (1/2/3): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        std::string filename;
        std::cout << "Enter binary file path: ";
        std::cin >> filename;
        
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file\n";
            return 1;
        }
        
        // Read binary file into program vector
        char byte;
        while (file.get(byte)) {
            program.push_back(static_cast<uint8_t>(byte));
        }
        file.close();
    }
    else if (choice == 2) {
        std::cout << "Enter instructions in hexadecimal (one byte per line)\n";
        std::cout << "Enter 'done' to finish input\n";
        
        std::string input;
        while (true) {
            std::cout << "> ";
            std::cin >> input;
            
            if (input == "done") break;
            
            // Convert hex string to byte
            try {
                uint8_t byte = std::stoi(input, nullptr, 16);
                program.push_back(byte);
            }
            catch (const std::exception& e) {
                std::cout << "Invalid input. Please enter hexadecimal values.\n";
            }
        }
    }
    else if (choice == 3) {
        std::cout << "Enter instructions using mnemonics (e.g., MOV_B_C)\n";
        std::cout << "Enter 'done' to finish input\n";
        
        // Map to store mnemonic to opcode conversions
        std::map<std::string, uint8_t> mnemonic_map = {
            // Data Transfer Instructions
            {"MOV_B_B", 0x40}, {"MOV_B_C", 0x41}, {"MOV_B_D", 0x42}, {"MOV_B_E", 0x43},
            {"MOV_B_H", 0x44}, {"MOV_B_L", 0x45}, {"MOV_B_M", 0x46}, {"MOV_B_A", 0x47},
            {"MOV_C_B", 0x48}, {"MOV_C_C", 0x49}, {"MOV_C_D", 0x4A}, {"MOV_C_E", 0x4B},
            {"MOV_C_H", 0x4C}, {"MOV_C_L", 0x4D}, {"MOV_C_M", 0x4E}, {"MOV_C_A", 0x4F},
            {"MOV_D_B", 0x50}, {"MOV_D_C", 0x51}, {"MOV_D_D", 0x52}, {"MOV_D_E", 0x53},
            {"MOV_D_H", 0x54}, {"MOV_D_L", 0x55}, {"MOV_D_M", 0x56}, {"MOV_D_A", 0x57},
            {"MOV_E_B", 0x58}, {"MOV_E_C", 0x59}, {"MOV_E_D", 0x5A}, {"MOV_E_E", 0x5B},
            {"MOV_E_H", 0x5C}, {"MOV_E_L", 0x5D}, {"MOV_E_M", 0x5E}, {"MOV_E_A", 0x5F},
            {"MOV_H_B", 0x60}, {"MOV_H_C", 0x61}, {"MOV_H_D", 0x62}, {"MOV_H_E", 0x63},
            {"MOV_H_H", 0x64}, {"MOV_H_L", 0x65}, {"MOV_H_M", 0x66}, {"MOV_H_A", 0x67},
            {"MOV_L_B", 0x68}, {"MOV_L_C", 0x69}, {"MOV_L_D", 0x6A}, {"MOV_L_E", 0x6B},
            {"MOV_L_H", 0x6C}, {"MOV_L_L", 0x6D}, {"MOV_L_M", 0x6E}, {"MOV_L_A", 0x6F},
            {"MOV_M_B", 0x70}, {"MOV_M_C", 0x71}, {"MOV_M_D", 0x72}, {"MOV_M_E", 0x73},
            {"MOV_M_H", 0x74}, {"MOV_M_L", 0x75}, {"MOV_M_A", 0x77},
            {"MOV_A_B", 0x78}, {"MOV_A_C", 0x79}, {"MOV_A_D", 0x7A}, {"MOV_A_E", 0x7B},
            {"MOV_A_H", 0x7C}, {"MOV_A_L", 0x7D}, {"MOV_A_M", 0x7E}, {"MOV_A_A", 0x7F},

            // Immediate Data Transfer
            {"MVI_B", 0x06}, {"MVI_C", 0x0E}, {"MVI_D", 0x16}, {"MVI_E", 0x1E},
            {"MVI_H", 0x26}, {"MVI_L", 0x2E}, {"MVI_M", 0x36}, {"MVI_A", 0x3E},

            // Load/Store Instructions
            {"LXI_B", 0x01}, {"LXI_D", 0x11}, {"LXI_H", 0x21}, {"LXI_SP", 0x31},
            {"LDAX_B", 0x0A}, {"LDAX_D", 0x1A},
            {"STAX_B", 0x02}, {"STAX_D", 0x12},
            {"LDA", 0x3A}, {"STA", 0x32},
            {"LHLD", 0x2A}, {"SHLD", 0x22},
            {"XCHG", 0xEB},

            // Arithmetic Instructions
            {"ADD_B", 0x80}, {"ADD_C", 0x81}, {"ADD_D", 0x82}, {"ADD_E", 0x83},
            {"ADD_H", 0x84}, {"ADD_L", 0x85}, {"ADD_M", 0x86}, {"ADD_A", 0x87},
            {"ADI", 0xC6},
            {"ADC_B", 0x88}, {"ADC_C", 0x89}, {"ADC_D", 0x8A}, {"ADC_E", 0x8B},
            {"ADC_H", 0x8C}, {"ADC_L", 0x8D}, {"ADC_M", 0x8E}, {"ADC_A", 0x8F},
            {"ACI", 0xCE},
            {"SUB_B", 0x90}, {"SUB_C", 0x91}, {"SUB_D", 0x92}, {"SUB_E", 0x93},
            {"SUB_H", 0x94}, {"SUB_L", 0x95}, {"SUB_M", 0x96}, {"SUB_A", 0x97},
            {"SUI", 0xD6},
            {"SBB_B", 0x98}, {"SBB_C", 0x99}, {"SBB_D", 0x9A}, {"SBB_E", 0x9B},
            {"SBB_H", 0x9C}, {"SBB_L", 0x9D}, {"SBB_M", 0x9E}, {"SBB_A", 0x9F},
            {"SBI", 0xDE},
            {"INR_B", 0x04}, {"INR_C", 0x0C}, {"INR_D", 0x14}, {"INR_E", 0x1C},
            {"INR_H", 0x24}, {"INR_L", 0x2C}, {"INR_M", 0x34}, {"INR_A", 0x3C},
            {"DCR_B", 0x05}, {"DCR_C", 0x0D}, {"DCR_D", 0x15}, {"DCR_E", 0x1D},
            {"DCR_H", 0x25}, {"DCR_L", 0x2D}, {"DCR_M", 0x35}, {"DCR_A", 0x3D},
            {"INX_B", 0x03}, {"INX_D", 0x13}, {"INX_H", 0x23}, {"INX_SP", 0x33},
            {"DCX_B", 0x0B}, {"DCX_D", 0x1B}, {"DCX_H", 0x2B}, {"DCX_SP", 0x3B},
            {"DAD_B", 0x09}, {"DAD_D", 0x19}, {"DAD_H", 0x29}, {"DAD_SP", 0x39},
            {"DAA", 0x27},

            // Logical Instructions
            {"ANA_B", 0xA0}, {"ANA_C", 0xA1}, {"ANA_D", 0xA2}, {"ANA_E", 0xA3},
            {"ANA_H", 0xA4}, {"ANA_L", 0xA5}, {"ANA_M", 0xA6}, {"ANA_A", 0xA7},
            {"ANI", 0xE6},
            {"XRA_B", 0xA8}, {"XRA_C", 0xA9}, {"XRA_D", 0xAA}, {"XRA_E", 0xAB},
            {"XRA_H", 0xAC}, {"XRA_L", 0xAD}, {"XRA_M", 0xAE}, {"XRA_A", 0xAF},
            {"XRI", 0xEE},
            {"ORA_B", 0xB0}, {"ORA_C", 0xB1}, {"ORA_D", 0xB2}, {"ORA_E", 0xB3},
            {"ORA_H", 0xB4}, {"ORA_L", 0xB5}, {"ORA_M", 0xB6}, {"ORA_A", 0xB7},
            {"ORI", 0xF6},
            {"CMP_B", 0xB8}, {"CMP_C", 0xB9}, {"CMP_D", 0xBA}, {"CMP_E", 0xBB},
            {"CMP_H", 0xBC}, {"CMP_L", 0xBD}, {"CMP_M", 0xBE}, {"CMP_A", 0xBF},
            {"CPI", 0xFE},
            {"RLC", 0x07}, {"RRC", 0x0F}, {"RAL", 0x17}, {"RAR", 0x1F},
            {"CMA", 0x2F}, {"CMC", 0x3F}, {"STC", 0x37},

            // Branch Instructions
            {"JMP", 0xC3}, {"JC", 0xDA}, {"JNC", 0xD2}, {"JZ", 0xCA}, {"JNZ", 0xC2},
            {"JM", 0xFA}, {"JP", 0xF2}, {"JPE", 0xEA}, {"JPO", 0xE2},
            {"CALL", 0xCD}, {"CC", 0xDC}, {"CNC", 0xD4}, {"CZ", 0xCC}, {"CNZ", 0xC4},
            {"CM", 0xFC}, {"CP", 0xF4}, {"CPE", 0xEC}, {"CPO", 0xE4},
            {"RET", 0xC9}, {"RC", 0xD8}, {"RNC", 0xD0}, {"RZ", 0xC8}, {"RNZ", 0xC0},
            {"RM", 0xF8}, {"RP", 0xF0}, {"RPE", 0xE8}, {"RPO", 0xE0},
            {"PCHL", 0xE9},
            {"RST_0", 0xC7}, {"RST_1", 0xCF}, {"RST_2", 0xD7}, {"RST_3", 0xDF},
            {"RST_4", 0xE7}, {"RST_5", 0xEF}, {"RST_6", 0xF7}, {"RST_7", 0xFF},

            // Stack Instructions
            {"PUSH_B", 0xC5}, {"PUSH_D", 0xD5}, {"PUSH_H", 0xE5}, {"PUSH_PSW", 0xF5},
            {"POP_B", 0xC1}, {"POP_D", 0xD1}, {"POP_H", 0xE1}, {"POP_PSW", 0xF1},
            {"XTHL", 0xE3}, {"SPHL", 0xF9},

            // Machine Control Instructions
            {"NOP", 0x00}, {"HLT", 0x76},
            {"DI", 0xF3}, {"EI", 0xFB},
            {"RIM", 0x20}, {"SIM", 0x30}
        };
        
        std::string input;
        while (true) {
            std::cout << "> ";
            std::cin >> input;
            
            if (input == "done") break;
            
            auto it = mnemonic_map.find(input);
            if (it != mnemonic_map.end()) {
                program.push_back(it->second);
            } else {
               try {
                uint8_t byte = std::stoi(input, nullptr, 16);
                program.push_back(byte);
            }
            catch (const std::exception& e) {
                std::cout << "Invalid input. Please enter hexadecimal values.\n";
            }
            }
        }
    }
    else {
        std::cerr << "Invalid choice\n";
        return 1;
    }
    
    if (program.empty()) {
        std::cerr << "Error: No program loaded\n";
        return 1;
    }
    
    sim.load_program(program);
    sim.run();
    sim.print_registers();
    
    return 0;
}