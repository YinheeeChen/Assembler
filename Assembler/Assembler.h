
#ifndef TESTMANCHESTER_ASSEMBLER_H
#define TESTMANCHESTER_ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Assembler {
public:
    void assemble(const string &fileName);

private:
    void loadAssemblyCode(const string &fileName);

    void processLine(const string &line);

    void buildSymbolTable();

    void generateMachineCode();

    vector<char> translateInstruction(string &opcode, string &operand);

    void writeMachineCode(const string &fileName);

    int getOpcodeBinary(string &opcode);

    template<typename T>
    void printLine(vector<T> toPrint, bool a);

    vector<char> decToBin(long long int integer, int size);

    int findSymbolAddress(const string &label);

    template<typename T>
    void printContent(vector<vector<T>> content);

    void printMapValues(const map<string, int> &myMap);

    bool turnInteger(string &token, int &value);

    void printCorresponding();

    // The map to  store symbols
    map<string, int> symbolTable;
    // A 2D vector to store assembly code
    vector<vector<string>> toAssemble;
    // A 2D vector to store machine code
    vector<vector<char>> assembled;
};


#endif //TESTMANCHESTER_ASSEMBLER_H
