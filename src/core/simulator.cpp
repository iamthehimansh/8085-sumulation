#include "../../include/simulator.hpp"
#include "../../include/instructions.hpp"
#include <iostream>
Simulator::Simulator() : memory(65536), cpu(memory) {
}

void Simulator::load_program(const std::vector<uint8_t>& program, uint16_t start_address) {
    for (size_t i = 0; i < program.size(); ++i) {
        memory.write(start_address + i, program[i]);
    }
    print_memory(start_address, start_address + program.size());
}

void Simulator::reset() {
    cpu.reset();
}

void Simulator::step() {
    cpu.print_registers();
    // get \n from user
    std::cout<<"Press enter to continue"<<std::endl;
    std::cin.get();

    std::cout<<"PC: "<<std::hex<<(int)cpu.PC<<std::endl;
    uint8_t opcode = memory.read(cpu.PC++);
    uint8_t operand1 = 0;
    uint8_t operand2 = 0;

    // Handle multi-byte instructions
    switch(opcode) {
        case LXI_B: case LXI_D: case LXI_H: case LXI_SP:
            operand1 = memory.read(cpu.PC++);
            operand2 = memory.read(cpu.PC++);
            cpu.execute_instruction(opcode, operand1, operand2);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            std::cout<<"operand2: "<<(int)operand2<<std::endl;
            break;

        // Memory operations with 16-bit address
        case LDA: case STA: case LHLD: case SHLD:
            operand1 = memory.read(cpu.PC++);
            operand2 = memory.read(cpu.PC++);
            cpu.execute_instruction(opcode, operand1, operand2);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            std::cout<<"operand2: "<<(int)operand2<<std::endl;
            break;

        // Register indirect memory operations
        case LDAX_B: case LDAX_D:
        case STAX_B: case STAX_D:
        case XCHG:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;
        case MOV_B_B: case MOV_B_C: case MOV_B_D: case MOV_B_E:
        case MOV_B_H: case MOV_B_L: case MOV_B_M: case MOV_B_A:
        case MOV_C_B: case MOV_C_C: case MOV_C_D: case MOV_C_E:
        case MOV_C_H: case MOV_C_L: case MOV_C_M: case MOV_C_A:
        case MOV_D_B: case MOV_D_C: case MOV_D_D: case MOV_D_E:
        case MOV_D_H: case MOV_D_L: case MOV_D_M: case MOV_D_A:
        case MOV_E_B: case MOV_E_C: case MOV_E_D: case MOV_E_E:
        case MOV_E_H: case MOV_E_L: case MOV_E_M: case MOV_E_A:
        case MOV_H_B: case MOV_H_C: case MOV_H_D: case MOV_H_E:
        case MOV_H_H: case MOV_H_L: case MOV_H_M: case MOV_H_A:
        case MOV_L_B: case MOV_L_C: case MOV_L_D: case MOV_L_E:
        case MOV_L_H: case MOV_L_L: case MOV_L_M: case MOV_L_A:
        case MOV_M_B: case MOV_M_C: case MOV_M_D: case MOV_M_E:
        case MOV_M_H: case MOV_M_L: case MOV_M_A:
        case MOV_A_B: case MOV_A_C: case MOV_A_D: case MOV_A_E:
        case MOV_A_H: case MOV_A_L: case MOV_A_M: case MOV_A_A:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;
        case ADD_B: case ADD_C: case ADD_D: case ADD_E:
        case ADD_H: case ADD_L: case ADD_M: case ADD_A:
        case ADC_B: case ADC_C: case ADC_D: case ADC_E:
        case ADC_H: case ADC_L: case ADC_M: case ADC_A:
        case SUB_B: case SUB_C: case SUB_D: case SUB_E:
        case SUB_H: case SUB_L: case SUB_M: case SUB_A:
        case SBB_B: case SBB_C: case SBB_D: case SBB_E:
        case SBB_H: case SBB_L: case SBB_M: case SBB_A:
        case INR_B: case INR_C: case INR_D: case INR_E:
        case INR_H: case INR_L: case INR_M: case INR_A:
        case DCR_B: case DCR_C: case DCR_D: case DCR_E:
        case DCR_H: case DCR_L: case DCR_M: case DCR_A:
        case INX_B: case INX_D: case INX_H: case INX_SP:
        case DCX_B: case DCX_D: case DCX_H: case DCX_SP:
        case DAD_B: case DAD_D: case DAD_H: case DAD_SP:
        case DAA:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;
        // Immediate Arithmetic Instructions
        case ADI: case ACI: case SUI: case SBI:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;


        //  imidiate
        case MVI_B:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;
        case MVI_A:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;

        case MVI_C:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;
        case MVI_D:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;

        case MVI_E:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;

        case MVI_H:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;
        case MVI_L:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;
        case MVI_M:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;
        // Logical Instructions with registers
        case ANA_B: case ANA_C: case ANA_D: case ANA_E:
        case ANA_H: case ANA_L: case ANA_M: case ANA_A:
        case XRA_B: case XRA_C: case XRA_D: case XRA_E:
        case XRA_H: case XRA_L: case XRA_M: case XRA_A:
        case ORA_B: case ORA_C: case ORA_D: case ORA_E:
        case ORA_H: case ORA_L: case ORA_M: case ORA_A:
        case CMP_B: case CMP_C: case CMP_D: case CMP_E:
        case CMP_H: case CMP_L: case CMP_M: case CMP_A:
        case RLC: case RRC: case RAL: case RAR:
        case CMA: case CMC: case STC:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;

        // Immediate Logical Instructions
        case ANI: case XRI: case ORI: case CPI:
            operand1 = memory.read(cpu.PC++);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            cpu.execute_instruction(opcode, operand1);
            break;

        // Branch Instructions with address
        case JMP: case JC: case JNC: case JZ: case JNZ:
        case JM: case JP: case JPE: case JPO:
        case CALL: case CC: case CNC: case CZ: case CNZ:
        case CM: case CP: case CPE: case CPO:
            operand1 = memory.read(cpu.PC++);
            operand2 = memory.read(cpu.PC++);
            cpu.execute_instruction(opcode, operand1, operand2);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            std::cout<<"operand1: "<<(int)operand1<<std::endl;
            std::cout<<"operand2: "<<(int)operand2<<std::endl;
            break;

        // Return Instructions
        case RET: case RC: case RNC: case RZ: case RNZ:
        case RM: case RP: case RPE: case RPO:
        case PCHL:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;

        // Restart Instructions
        case RST_0: case RST_1: case RST_2: case RST_3:
        case RST_4: case RST_5: case RST_6: case RST_7:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;
        // Stack Instructions
        case PUSH_B: case PUSH_D: case PUSH_H: case PUSH_PSW:
        case POP_B: case POP_D: case POP_H: case POP_PSW:
        case XTHL: case SPHL:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;

        // Machine Control Instructions
        case NOP: case HLT:
        case DI: case EI:
        case RIM: case SIM:
            cpu.execute_instruction(opcode);
            std::cout<<"opcode: "<<(int)opcode<<std::endl;
            break;
        default:
            // operand1 = memory.read(cpu.PC++);
            // std::cout<<"opcode: "<<(int)opcode<<std::endl;
            // std::cout<<"operand1: "<<(int)operand1<<std::endl;
            // cpu.execute_instruction(opcode, operand1);
            std::cout<<"Error unknown opcode: "<<(int)opcode<<std::endl;
            break;
    }
    
}

void Simulator::run() {
    while (cpu.Halted==0) {
        step();
    }
}

void Simulator::print_registers() {
    cpu.print_registers();
}

void Simulator::print_memory(uint16_t start, uint16_t end) {
    cpu.print_memory(start, end);
}