# Assembler

## Introduction
Assembler is a C++ project designed to simulate the functionality of an assembler. This project converts assembly language code into machine code, providing a fundamental understanding of low-level programming and system operations.

## Features
- **Assembly Language Parsing**: Parses assembly language instructions.
- **Machine Code Generation**: Converts parsed assembly instructions into machine code.
- **Error Handling**: Identifies and reports errors in the assembly code.

## Installation
To install and run this project, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/YinheeeChen/Assembler.git
   cd Assembler
   ```

2. Compile the code:
   ```sh
   g++ -o assembler main.cpp

## Usage
To use the assembler, follow these instructions:

1. Prepare your assembly code in a text file (e.g., `program.asm`).
2. Run the assembler with the assembly file as an argument:
   ```sh
   ./assembler program.asm
   ```
3. The output machine code will be generated and displayed.

## Code Structure
- **main.cpp**: Contains the main function which initiates the assembling process.
- **Assembler.h**: Defines the Assembler class with methods for loading assembly code, processing lines, building the symbol table, generating machine code, and writing the machine code to a file.

## Methods in Assembler.h
- `void assemble(const string &fileName)`: Main function to start the assembly process.
- `void loadAssemblyCode(const string &fileName)`: Loads assembly code from the file.
- `void processLine(const string &line)`: Processes a single line of assembly code.
- `void buildSymbolTable()`: Builds the symbol table from the assembly code.
- `void generateMachineCode()`: Generates machine code from the assembly code.
- `vector<char> translateInstruction(string &opcode, string &operand)`: Translates an instruction to machine code.
- `void writeMachineCode(const string &fileName)`: Writes the machine code to a file.
- `int getOpcodeBinary(string &opcode)`: Converts an opcode to its binary representation.
- `vector<char> decToBin(long long int integer, int size)`: Converts a decimal number to binary.
- `int findSymbolAddress(const string &label)`: Finds the address of a symbol in the symbol table.
- `bool turnInteger(string &token, int &value)`: Converts a string token to an integer.
- `void printCorresponding()`: Prints corresponding values (for debugging purposes).

## Contact
If you have any questions or suggestions, feel free to contact me at [your email or contact information].
