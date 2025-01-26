#include "../../include/cpu.hpp"
#include "../../include/instructions.hpp"
#include "../../include/utils.hpp"

CPU::CPU(Memory& mem) : memory(mem) {
    reset();
    // Initialize the new variables
    serial_in = 0;
    serial_out = 0;
    pending_intr5 = false;
    pending_intr6 = false;
    pending_intr7 = false;
    mask_intr5 = false;
    mask_intr6 = false;
    mask_intr7 = false;
    interrupts_enabled = false;
}



void CPU::reset() {
    A = B = C = D = E = H = L = 0;
    PC = 0;
    SP = 0xFFFF;
    flags.S = flags.Z = flags.AC = flags.P = flags.CY = 0;
    Halted = 0;
    interrupts_enabled = false;
    pending_intr5 = pending_intr6 = pending_intr7 = false;
    mask_intr5 = mask_intr6 = mask_intr7 = false;
    serial_in = serial_out = 0;
}

void CPU::update_flags(uint8_t value) {
    flags.Z = Utils::calculate_zero(value);
    flags.S = Utils::calculate_sign(value);
    flags.P = Utils::calculate_parity(value);
}

void CPU::execute_instruction(uint8_t opcode, uint8_t operand, uint8_t operand2) {
    switch (opcode) {
        // Data Transfer Instructions
        case MOV_B_B: B = B; break;
        case MOV_B_C: B = C; break;
        case MOV_B_D: B = D; break;
        case MOV_B_E: B = E; break;
        case MOV_B_H: B = H; break;
        case MOV_B_L: B = L; break;
        case MOV_B_M: B = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_B_A: B = A; break;

        case MOV_C_B: C = B; break;
        case MOV_C_C: C = C; break;
        case MOV_C_D: C = D; break;
        case MOV_C_E: C = E; break;
        case MOV_C_H: C = H; break;
        case MOV_C_L: C = L; break;
        case MOV_C_M: C = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_C_A: C = A; break;

        case MOV_D_B: D = B; break;
        case MOV_D_C: D = C; break;
        case MOV_D_D: D = D; break;
        case MOV_D_E: D = E; break;
        case MOV_D_H: D = H; break;
        case MOV_D_L: D = L; break;
        case MOV_D_M: D = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_D_A: D = A; break;

        case MOV_E_B: E = B; break;
        case MOV_E_C: E = C; break;
        case MOV_E_D: E = D; break;
        case MOV_E_E: E = E; break;
        case MOV_E_H: E = H; break;
        case MOV_E_L: E = L; break;
        case MOV_E_M: E = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_E_A: E = A; break;

        case MOV_H_B: H = B; break;
        case MOV_H_C: H = C; break;
        case MOV_H_D: H = D; break;
        case MOV_H_E: H = E; break;
        case MOV_H_H: H = H; break;
        case MOV_H_L: H = L; break;
        case MOV_H_M: H = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_H_A: H = A; break;

        case MOV_L_B: L = B; break;
        case MOV_L_C: L = C; break;
        case MOV_L_D: L = D; break;
        case MOV_L_E: L = E; break;
        case MOV_L_H: L = H; break;
        case MOV_L_L: L = L; break;
        case MOV_L_M: L = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_L_A: L = A; break;

        case MOV_M_B: memory.write(Utils::combine_bytes(H, L), B); break;
        case MOV_M_C: memory.write(Utils::combine_bytes(H, L), C); break;
        case MOV_M_D: memory.write(Utils::combine_bytes(H, L), D); break;
        case MOV_M_E: memory.write(Utils::combine_bytes(H, L), E); break;
        case MOV_M_H: memory.write(Utils::combine_bytes(H, L), H); break;
        case MOV_M_L: memory.write(Utils::combine_bytes(H, L), L); break;
        case MOV_M_A: memory.write(Utils::combine_bytes(H, L), A); break;

        case MOV_A_B: A = B; break;
        case MOV_A_C: A = C; break;
        case MOV_A_D: A = D; break;
        case MOV_A_E: A = E; break;
        case MOV_A_H: A = H; break;
        case MOV_A_L: A = L; break;
        case MOV_A_M: A = memory.read(Utils::combine_bytes(H, L)); break;
        case MOV_A_A: A = A; break;
        
        // Immediate Data Transfer
        case MVI_A: A = operand; break;
        case MVI_B: B = operand; break;
        case MVI_C: C = operand; break;
        case MVI_D: D = operand; break;
        case MVI_E: E = operand; break;
        case MVI_H: H = operand; break;
        case MVI_L: L = operand; break;



        // Arithmetic Instructions
        case ADD_B: case ADD_C: case ADD_D: case ADD_E:
        case ADD_H: case ADD_L: case ADD_M: case ADD_A: {
            uint8_t value;
            switch(opcode) {
                case ADD_B: value = B; break;
                case ADD_C: value = C; break;
                case ADD_D: value = D; break;
                case ADD_E: value = E; break;
                case ADD_H: value = H; break;
                case ADD_L: value = L; break;
                case ADD_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case ADD_A: value = A; break;
            }
            uint16_t result = A + value;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) + (value & 0x0F)) > 0x0F;
            A = result & 0xFF;
            update_flags(A);
            break;
        }

        case ADC_B: case ADC_C: case ADC_D: case ADC_E:
        case ADC_H: case ADC_L: case ADC_M: case ADC_A: {
            uint8_t value;
            switch(opcode) {
                case ADC_B: value = B; break;
                case ADC_C: value = C; break;
                case ADC_D: value = D; break;
                case ADC_E: value = E; break;
                case ADC_H: value = H; break;
                case ADC_L: value = L; break;
                case ADC_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case ADC_A: value = A; break;
            }
            uint16_t result = A + value + flags.CY;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) + (value & 0x0F) + flags.CY) > 0x0F;
            A = result & 0xFF;
            update_flags(A);
            break;
        }

        case SUB_B: case SUB_C: case SUB_D: case SUB_E:
        case SUB_H: case SUB_L: case SUB_M: case SUB_A: {
            uint8_t value;
            switch(opcode) {
                case SUB_B: value = B; break;
                case SUB_C: value = C; break;
                case SUB_D: value = D; break;
                case SUB_E: value = E; break;
                case SUB_H: value = H; break;
                case SUB_L: value = L; break;
                case SUB_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case SUB_A: value = A; break;
            }
            uint16_t result = A - value;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (value & 0x0F)) < 0;
            A = result & 0xFF;
            update_flags(A);
            break;
        }

        case SBB_B: case SBB_C: case SBB_D: case SBB_E:
        case SBB_H: case SBB_L: case SBB_M: case SBB_A: {
            uint8_t value;
            switch(opcode) {
                case SBB_B: value = B; break;
                case SBB_C: value = C; break;
                case SBB_D: value = D; break;
                case SBB_E: value = E; break;
                case SBB_H: value = H; break;
                case SBB_L: value = L; break;
                case SBB_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case SBB_A: value = A; break;
            }
            uint16_t result = A - value - flags.CY;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (value & 0x0F) - flags.CY) < 0;
            A = result & 0xFF;
            update_flags(A);
            break;
        }

        case INR_B: case INR_C: case INR_D: case INR_E:
        case INR_H: case INR_L: case INR_M: case INR_A: {
            uint8_t* reg;
            switch(opcode) {
                case INR_B: reg = &B; break;
                case INR_C: reg = &C; break;
                case INR_D: reg = &D; break;
                case INR_E: reg = &E; break;
                case INR_H: reg = &H; break;
                case INR_L: reg = &L; break;
                case INR_A: reg = &A; break;
                case INR_M: {
                    uint16_t addr = Utils::combine_bytes(H, L);
                    uint8_t value = memory.read(addr);
                    flags.AC = ((value & 0x0F) + 1) > 0x0F;
                    value++;
                    memory.write(addr, value);
                    update_flags(value);
                    break;
                }
                default: break;
            }
            if (opcode != INR_M) {
                flags.AC = ((*reg & 0x0F) + 1) > 0x0F;
                (*reg)++;
                update_flags(*reg);
            }
            break;
        }

        case DCR_B: case DCR_C: case DCR_D: case DCR_E:
        case DCR_H: case DCR_L: case DCR_M: case DCR_A: {
            uint8_t* reg;
            switch(opcode) {
                case DCR_B: reg = &B; break;
                case DCR_C: reg = &C; break;
                case DCR_D: reg = &D; break;
                case DCR_E: reg = &E; break;
                case DCR_H: reg = &H; break;
                case DCR_L: reg = &L; break;
                case DCR_A: reg = &A; break;
                case DCR_M: {
                    uint16_t addr = Utils::combine_bytes(H, L);
                    uint8_t value = memory.read(addr);
                    flags.AC = ((value & 0x0F) - 1) < 0;
                    value--;
                    memory.write(addr, value);
                    update_flags(value);
                    break;
                }
                default: break;
            }
            if (opcode != DCR_M) {
                flags.AC = ((*reg & 0x0F) - 1) < 0;
                (*reg)--;
                update_flags(*reg);
            }
            break;
        }

        case INX_B: {
            uint16_t bc = Utils::combine_bytes(B, C);
            bc++;
            B = (bc >> 8) & 0xFF;
            C = bc & 0xFF;
            break;
        }
        case INX_D: {
            uint16_t de = Utils::combine_bytes(D, E);
            de++;
            D = (de >> 8) & 0xFF;
            E = de & 0xFF;
            break;
        }
        case INX_H: {
            uint16_t hl = Utils::combine_bytes(H, L);
            hl++;
            H = (hl >> 8) & 0xFF;
            L = hl & 0xFF;
            break;
        }
        case INX_SP: {
            SP++;
            break;
        }

        case DCX_B: {
            uint16_t bc = Utils::combine_bytes(B, C);
            bc--;
            B = (bc >> 8) & 0xFF;
            C = bc & 0xFF;
            break;
        }
        case DCX_D: {
            uint16_t de = Utils::combine_bytes(D, E);
            de--;
            D = (de >> 8) & 0xFF;
            E = de & 0xFF;
            break;
        }
        case DCX_H: {
            uint16_t hl = Utils::combine_bytes(H, L);
            hl--;
            H = (hl >> 8) & 0xFF;
            L = hl & 0xFF;
            break;
        }
        case DCX_SP: {
            SP--;
            break;
        }

        case DAD_B: {
            uint32_t result = Utils::combine_bytes(H, L) + Utils::combine_bytes(B, C);
            flags.CY = (result > 0xFFFF);
            H = (result >> 8) & 0xFF;
            L = result & 0xFF;
            break;
        }
        case DAD_D: {
            uint32_t result = Utils::combine_bytes(H, L) + Utils::combine_bytes(D, E);
            flags.CY = (result > 0xFFFF);
            H = (result >> 8) & 0xFF;
            L = result & 0xFF;
            break;
        }
        case DAD_H: {
            uint32_t result = Utils::combine_bytes(H, L) + Utils::combine_bytes(H, L);
            flags.CY = (result > 0xFFFF);
            H = (result >> 8) & 0xFF;
            L = result & 0xFF;
            break;
        }
        case DAD_SP: {
            uint32_t result = Utils::combine_bytes(H, L) + SP;
            flags.CY = (result > 0xFFFF);
            H = (result >> 8) & 0xFF;
            L = result & 0xFF;
            break;
        }

        case DAA: {
            uint8_t correction = 0;
            uint8_t lsb = A & 0x0F;
            uint8_t msb = (A >> 4) & 0x0F;
            
            if (flags.AC || lsb > 9) {
                correction += 0x06;
            }
            if (flags.CY || msb > 9 || (msb == 9 && lsb > 9)) {
                correction += 0x60;
                flags.CY = 1;
            }
            
            uint16_t result = A + correction;
            A = result & 0xFF;
            update_flags(A);
            break;
        }
        case ADI: {
            uint16_t result = A + operand;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) + (operand & 0x0F)) > 0x0F;
            A = result & 0xFF;
            update_flags(A);
            break;
        }
        case ACI: {
            uint16_t result = A + operand + flags.CY;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) + (operand & 0x0F) + flags.CY) > 0x0F;
            A = result & 0xFF;
            update_flags(A);
            break;
        }
        case SUI: {
            uint16_t result = A - operand;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (operand & 0x0F)) < 0;
            A = result & 0xFF;
            update_flags(A);
            break;
        }
        case SBI: {
            uint16_t result = A - operand - flags.CY;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (operand & 0x0F) - flags.CY) < 0;
            A = result & 0xFF;
            update_flags(A);
            break;
        }


        // Logical Instructions
        case ANA_B: case ANA_C: case ANA_D: case ANA_E:
        case ANA_H: case ANA_L: case ANA_M: case ANA_A: {
            uint8_t value;
            switch(opcode) {
                case ANA_B: value = B; break;
                case ANA_C: value = C; break;
                case ANA_D: value = D; break;
                case ANA_E: value = E; break;
                case ANA_H: value = H; break;
                case ANA_L: value = L; break;
                case ANA_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case ANA_A: value = A; break;
            }
            A &= value;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }

        case XRA_B: case XRA_C: case XRA_D: case XRA_E:
        case XRA_H: case XRA_L: case XRA_M: case XRA_A: {
            uint8_t value;
            switch(opcode) {
                case XRA_B: value = B; break;
                case XRA_C: value = C; break;
                case XRA_D: value = D; break;
                case XRA_E: value = E; break;
                case XRA_H: value = H; break;
                case XRA_L: value = L; break;
                case XRA_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case XRA_A: value = A; break;
            }
            A ^= value;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }

        case ORA_B: case ORA_C: case ORA_D: case ORA_E:
        case ORA_H: case ORA_L: case ORA_M: case ORA_A: {
            uint8_t value;
            switch(opcode) {
                case ORA_B: value = B; break;
                case ORA_C: value = C; break;
                case ORA_D: value = D; break;
                case ORA_E: value = E; break;
                case ORA_H: value = H; break;
                case ORA_L: value = L; break;
                case ORA_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case ORA_A: value = A; break;
            }
            A |= value;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }

        case CMP_B: case CMP_C: case CMP_D: case CMP_E:
        case CMP_H: case CMP_L: case CMP_M: case CMP_A: {
            uint8_t value;
            switch(opcode) {
                case CMP_B: value = B; break;
                case CMP_C: value = C; break;
                case CMP_D: value = D; break;
                case CMP_E: value = E; break;
                case CMP_H: value = H; break;
                case CMP_L: value = L; break;
                case CMP_M: value = memory.read(Utils::combine_bytes(H, L)); break;
                case CMP_A: value = A; break;
            }
            uint16_t result = A - value;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (value & 0x0F)) < 0;
            update_flags(result & 0xFF);
            break;
        }

        // Restart Instructions
        case RST_0: case RST_1: case RST_2: case RST_3:
        case RST_4: case RST_5: case RST_6: case RST_7: {
            SP -= 2;
            memory.write(SP, PC & 0xFF);
            memory.write(SP + 1, (PC >> 8) & 0xFF);
            PC = (opcode & 0x38);  // Extract restart address from opcode
            break;
        }

       
        
        case PUSH_PSW: {
            uint8_t psw = (A << 8) | 
                         (flags.S << 7) |
                         (flags.Z << 6) |
                         (flags.AC << 4) |
                         (flags.P << 2) |
                         (1 << 1) |     // Bit 1 is always 1
                         flags.CY;
            SP -= 2;
            memory.write(SP, psw);
            memory.write(SP + 1, A);
            break;
        }
        case POP_B: {
            B = memory.read(SP + 1);
            C = memory.read(SP);
            SP += 2;
            break;
        }
        case POP_D: {
            D = memory.read(SP + 1);
            E = memory.read(SP);
            SP += 2;
            break;
        }
        case POP_H: {
            H = memory.read(SP + 1);
            L = memory.read(SP);
            SP += 2;
            break;
        }
        case POP_PSW: {
            A = memory.read(SP + 1);
            uint8_t psw = memory.read(SP);
            flags.S = (psw >> 7) & 1;
            flags.Z = (psw >> 6) & 1;
            flags.AC = (psw >> 4) & 1;
            flags.P = (psw >> 2) & 1;
            flags.CY = psw & 1;
            SP += 2;
            break;
        }
        case XTHL: {
            uint8_t temp_l = memory.read(SP);
            uint8_t temp_h = memory.read(SP + 1);
            memory.write(SP, L);
            memory.write(SP + 1, H);
            H = temp_h;
            L = temp_l;
            break;
        }
        case SPHL: {
            SP = Utils::combine_bytes(H, L);
            break;
        }

        // Machine Control Instructions
        
        case DI: {
            interrupts_enabled = false;
            break;
        }
        case EI: {
            interrupts_enabled = true;
            break;
        }
        case RIM: {
            // Read Interrupt Mask and Serial Input
            std::cout << "Enter a bit (0 or 1): ";
            serial_in = std::cin.get() & 0x01;  // Read one bit from stdin
            A = (interrupts_enabled << 7) |
                (pending_intr5 << 6) |
                (pending_intr6 << 5) |
                (pending_intr7 << 4) |
                (mask_intr5 << 3) |
                (mask_intr6 << 2) |
                (mask_intr7 << 1) |
                serial_in;
            break;
        }
        case SIM: {
            // Set Interrupt Mask and Serial Output
            if (A & 0x40) {  // If bit 6 is set, set serial output
                serial_out = A & 1;
                std::cout.put(serial_out ? '1' : '0');  // Output one bit to stdout
                std::cout.flush();
            }
            if (A & 0x08) {  // If bit 3 is set, update masks
                mask_intr5 = (A >> 3) & 1;
                mask_intr6 = (A >> 2) & 1;
                mask_intr7 = (A >> 1) & 1;
            }
            if (A & 0x10) {  // If bit 4 is set, reset pending interrupts
                pending_intr5 &= ~((A >> 3) & 1);
                pending_intr6 &= ~((A >> 2) & 1);
                pending_intr7 &= ~((A >> 1) & 1);
            }
            break;
        }
        case PCHL: {
            PC = Utils::combine_bytes(H, L);
            break;
        }

        // Rotate Instructions
        case RLC: {
            flags.CY = (A & 0x80) >> 7;
            A = (A << 1) | flags.CY;
            break;
        }
        case RRC: {
            flags.CY = A & 0x01;
            A = (A >> 1) | (flags.CY << 7);
            break;
        }
        case RAL: {
            uint8_t old_cy = flags.CY;
            flags.CY = (A & 0x80) >> 7;
            A = (A << 1) | old_cy;
            break;
        }
        case RAR: {
            uint8_t old_cy = flags.CY;
            flags.CY = A & 0x01;
            A = (A >> 1) | (old_cy << 7);
            break;
        }

        // Flag Instructions
        case CMA: {
            A = ~A;
            break;
        }
        case CMC: {
            flags.CY = !flags.CY;
            break;
        }
        case STC: {
            flags.CY = 1;
            break;
        }

        // Load/Store Instructions
        case LXI_B: {
            B = operand2;
            C = operand;
            break;
        }
        case LXI_D: {
            D = operand2;
            E = operand;
            break;
        }
        case LXI_H: {
            H = operand2;
            L = operand;
            break;
        }
        case LXI_SP: {
            SP = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case LDAX_B: {
            A = memory.read(Utils::combine_bytes(B, C));
            break;
        }
        case LDAX_D: {
            A = memory.read(Utils::combine_bytes(D, E));
            break;
        }
        case STAX_B: {
            memory.write(Utils::combine_bytes(B, C), A);
            break;
        }
        case STAX_D: {
            memory.write(Utils::combine_bytes(D, E), A);
            break;
        }
        case LDA: {
            uint16_t address = (static_cast<uint16_t>(operand2) << 8) | operand;
            A = memory.read(address);
            break;
        }
        case LHLD: {
            uint16_t address = (static_cast<uint16_t>(operand2) << 8) | operand;
            L = memory.read(address);
            H = memory.read(address + 1);
            break;
        }
        case SHLD: {
            uint16_t address = (static_cast<uint16_t>(operand2) << 8) | operand;
            memory.write(address, L);
            memory.write(address + 1, H);
            break;
        }
        case XCHG: {
            uint8_t temp_h = H;
            uint8_t temp_l = L;
            H = D;
            L = E;
            D = temp_h;
            E = temp_l;
            break;
        }

        case PUSH_B: {
            uint16_t value = Utils::combine_bytes(B, C);
            SP -= 2;
            memory.write(SP, value & 0xFF);
            memory.write(SP + 1, (value >> 8));
            break;
        }
        case PUSH_D: {
            uint16_t value = Utils::combine_bytes(D, E);
            SP -= 2;
            memory.write(SP, value & 0xFF);
            memory.write(SP + 1, (value >> 8));
            break;
        }
        case PUSH_H: {
            uint16_t value = Utils::combine_bytes(H, L);
            SP -= 2;
            memory.write(SP, value & 0xFF);
            memory.write(SP + 1, (value >> 8));
            break;
        }
        case ANI: {
            A &= operand;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }
        case XRI: {
            A ^= operand;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }
        case ORI: {
            A |= operand;
            flags.CY = 0;
            flags.AC = 0;
            update_flags(A);
            break;
        }
        case CPI: {
            uint16_t result = A - operand;
            flags.CY = (result > 0xFF);
            flags.AC = ((A & 0x0F) - (operand & 0x0F)) < 0;
            update_flags(result & 0xFF);
            break;
        }
        case JMP: {
            PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JC: {
            if (flags.CY) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JNC: {
            if (!flags.CY) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JZ: {
            if (flags.Z) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JNZ: {
            if (!flags.Z) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JM: {
            if (flags.S) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JP: {
            if (!flags.S) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JPE: {
            if (flags.P) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case JPO: {
            if (!flags.P) PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }

        // Call Instructions
        case CALL: {
            SP -= 2;
            memory.write(SP, PC & 0xFF);
            memory.write(SP + 1, (PC >> 8) & 0xFF);
            PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            break;
        }
        case CC: {
            if (flags.CY) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CNC: {
            if (!flags.CY) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CZ: {
            if (flags.Z) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CNZ: {
            if (!flags.Z) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CM: {
            if (flags.S) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CP: {
            if (!flags.S) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CPE: {
            if (flags.P) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case CPO: {
            if (!flags.P) {
                SP -= 2;
                memory.write(SP, PC & 0xFF);
                memory.write(SP + 1, (PC >> 8) & 0xFF);
                PC = (static_cast<uint16_t>(operand2) << 8) | operand;
            }
            break;
        }
        case RET: {
            uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
            SP += 2;
            PC = addr;
            break;
        }
        case RC: {
            if (flags.CY) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RNC: {
            if (!flags.CY) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RZ: {
            if (flags.Z) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RNZ: {
            if (!flags.Z) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RM: {
            if (flags.S) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RP: {
            if (!flags.S) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RPE: {
            if (flags.P) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        case RPO: {
            if (!flags.P) {
                uint16_t addr = (static_cast<uint16_t>(memory.read(SP + 1)) << 8) | memory.read(SP);
                SP += 2;
                PC = addr;
            }
            break;
        }
        // Control Instructions
        case NOP: break;
        case STA: {
            uint16_t address = (static_cast<uint16_t>(operand2) << 8) | operand;
            memory.write(address, A);
            break;
        }
        case HLT: 
            Halted = 1;
            break;
    }
}

void CPU::print_registers() {
    // Implementation of printing registers

    std::cout<<"Registers:"<<std::endl;
    std::cout<<"A: "<<std::hex<<(int)A<<std::endl;
    std::cout<<"B: "<<std::hex<<(int)B<<std::endl;
    std::cout<<"C: "<<std::hex<<(int)C<<std::endl;
    std::cout<<"D: "<<std::hex<<(int)D<<std::endl;
    std::cout<<"E: "<<std::hex<<(int)E<<std::endl;
    std::cout<<"H: "<<std::hex<<(int)H<<std::endl;
    std::cout<<"L: "<<std::hex<<(int)L<<std::endl;
    std::cout<<"Flags:"<<std::endl;
    std::cout<<"S: "<<(bool)flags.S<<std::endl;
    std::cout<<"Z: "<<(bool)flags.Z<<std::endl;
    std::cout<<"AC: "<<(bool)flags.AC<<std::endl;
    std::cout<<"P: "<<(bool)flags.P<<std::endl;
    std::cout<<"CY: "<<(bool)flags.CY<<std::endl;
    
}
void CPU::print_memory(uint16_t start_address, uint16_t end_address) {
    // Implementation of printing memory
    std::cout<<"Memory:"<<std::endl;
    for (uint16_t i = start_address; i <= end_address; ++i) {
        std::cout<<std::hex<<i<<": "<<std::hex<<(int)memory.read(i)<<std::endl;
    }
}