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
            {"MOV_B_C", 0x41},
            {"MOV_B_D", 0x42},
            // Add more mnemonics here
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