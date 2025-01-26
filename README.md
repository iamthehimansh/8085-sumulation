# 8085 Microprocessor Simulator

A comprehensive simulator for the Intel 8085 microprocessor written in C++. This simulator provides accurate emulation of the 8085's instruction set and features.

## Features

- Complete implementation of 8085 instruction set
- Memory management (64KB addressable memory)
- Register operations
- Flag handling
- Multiple input methods:
  - Binary file loading
  - Hexadecimal input
  - Mnemonic-based input
- Step-by-step execution with register state display
- Serial I/O support

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the simulator:
```bash
./8085sim
```

The simulator provides three input methods:

1. Load from binary file
2. Input instructions manually (hexadecimal)
3. Input instructions using mnemonics

### Example Program

Here's a simple program that adds two numbers:
```assembly
MVI_A, 05    ; Load 5 into accumulator
MOV_B_A      ; Move accumulator to register B
MVI_A, 03    ; Load 3 into accumulator
ADD_B        ; Add B to accumulator
STA 2000     ; Store result at memory location 2000H
HLT          ; Halt program
```

## Supported Instructions

- Data Transfer Instructions (MOV, MVI, etc.)
- Arithmetic Instructions (ADD, SUB, INR, etc.)
- Logical Instructions (ANA, XRA, CMP, etc.)
- Branch Instructions (JMP, CALL, RET, etc.)
- Stack Operations (PUSH, POP)
- I/O and Machine Control Instructions

## License

This project is open source and available under the MIT License.

## Author

Himansh Raj:iamthehimansh

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
