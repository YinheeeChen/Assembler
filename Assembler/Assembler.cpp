
#include "Assembler.h"
#include <bitset>
#include <iomanip>

/**
 *  This method is the whole process of assembling.
 *  Called by Processer
 * @param fileName The file that contains assembly code
 */
void Assembler::assemble(const string &fileName)
{
    loadAssemblyCode(fileName);
    buildSymbolTable();
    printContent(toAssemble);
    printMapValues(symbolTable);
    generateMachineCode();
    printCorresponding();
    printContent(assembled);
    writeMachineCode("machine_code.txt");
}

/**
 * This method load assembly code from a file
 * @param fileName The file that contains the assembly code
 */
void Assembler::loadAssemblyCode(const string &fileName)
{
    ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open file " << fileName << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(inFile, line))
    {
        processLine(line);
    }
    inFile.close();
}

/**
 * This method processes each line and stores tokens
 * @param line Each line of assembly code
 */
void Assembler::processLine(const string &line)
{
    if (line.empty() || line[0] == ';')
    {
        return;
    }
    size_t commentPos = line.find(';');
    string cleaned;
    if (commentPos != string::npos)
    {
        cleaned = line.substr(0, commentPos);
    }
    else
    {
        cleaned = line;
    }
    if (!cleaned.empty())
    {
        vector<string> tokens;
        istringstream iss(cleaned);
        string token;
        while (iss >> token)
        {
            tokens.push_back(token);
        }
        if (!tokens.empty())
        {
            toAssemble.push_back(tokens);
        }
    }
}

/**
 * This method generate a corresponding symbol table for assembly code
 */
void Assembler::buildSymbolTable()
{
    int lineCount = 0;
    // 可以根据需要扩展加载符号表的功能
    for (size_t i = 0; i < toAssemble.size(); ++i)
    {
        auto &line = toAssemble[i];
        if (!line.empty())
        {
            string &firstElem = line[0];
            if (firstElem.back() == ':')
            {
                string label = firstElem.substr(0, firstElem.size() - 1);
                symbolTable[label] = lineCount; // 将行号作为地址值
                toAssemble[i].erase(line.begin());
            }
        }
        if (find(line.begin(), line.end(), "MOV") != line.end())
        {
            // Check if "MOV" is present in the line
            lineCount++;
        }
        lineCount++;
    }
}

/**
 * This method searches symbols from symbol table and return its corresponding value
 * @param label The label used to find the symbol
 * @return
 */
int Assembler::findSymbolAddress(const string &label)
{
    auto it = symbolTable.find(label);
    if (it != symbolTable.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}

/**
 * This method generates machine code from assembly code
 */
void Assembler::generateMachineCode()
{
    for (int i = 0; i < toAssemble.size(); i++)
    {
        if (!toAssemble[i].empty())
        {
            if (toAssemble[i][0] == "VAR")
            {
                if (toAssemble[i].size() != 2)
                {
                    cout << "Invalid Parameter Number." << endl;
                    exit(EXIT_FAILURE);
                }
                int value;
                if (turnInteger(toAssemble[i][1], value))
                    assembled.push_back(decToBin(value, 32));
            }
            else if (toAssemble[i][0] == "STP")
            {
                assembled.push_back(translateInstruction(toAssemble[i][0], (string &)"0"));
            }
            else if (toAssemble[i][0] == "MOV")
            {
                if (toAssemble[i].size() != 3)
                {
                    cout << "Invalid Parameter Number." << endl;
                    exit(EXIT_FAILURE);
                }
                if (toAssemble[i][1].front() == '#')
                {
                    assembled.push_back(translateInstruction(toAssemble[i][0], toAssemble[i][2]));
                    string token = toAssemble[i][1].substr(1);
                    int value;
                    if (turnInteger(token, value))
                        assembled.push_back(decToBin(value, 32));
                }
                else if (toAssemble[i][2].front() == '#')
                {
                    assembled.push_back(translateInstruction(toAssemble[i][0], toAssemble[i][1]));
                    string token = toAssemble[i][2].substr(1);
                    int value;
                    if (turnInteger(token, value))
                        assembled.push_back(decToBin(value, 32));
                }
                else
                {
                    cout << "Invalid Operand: " << toAssemble[i][1] << "," << toAssemble[i][2] << endl;
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                if (toAssemble[i].size() != 2)
                {
                    cout << "Invalid Parameter Number." << endl;
                    exit(EXIT_FAILURE);
                }
                assembled.push_back(translateInstruction(toAssemble[i][0], toAssemble[i][1]));
            }
        }
    }
}

/**
 *  This method generates corresponding machine code for opcodes and operands
 * @param opcode The assembly code of opcode
 * @param operand The assembly code of operand
 * @return
 */
vector<char>
Assembler::translateInstruction(string &opcode, string &operand)
{ // Converts into an Instruction line to binary
    vector<char> line(32, '0');
    vector<char> opcodeSeq(5, '0');
    vector<char> operandSeq(5, '0');

    int number = getOpcodeBinary(opcode);
    if (number == -1)
    {
        cout << "Invalid Instruction: " << opcode << endl;
        exit(EXIT_FAILURE);
    }

    opcodeSeq = decToBin(number, 5);

    if (getOpcodeBinary(opcode) != 7)
    {
        if (findSymbolAddress(operand) == -1)
        {
            cout << endl
                 << "Unknown Label: " << operand << endl;
            exit(EXIT_FAILURE);
        }
        int lineNumber = findSymbolAddress(operand);
        operandSeq = decToBin(lineNumber, 5);
        for (int i = 0; i < 5; i++)
        {
            line[i] = operandSeq[i];
        }
    }
    for (int i = 0; i < 5; i++)
    {
        line[i + 13] = opcodeSeq[i];
    }
    return line;
}

/**
 * This method return the corresponding decimal value
 * @param opcode The literal assembly code of opcode
 * @return
 */
int Assembler::getOpcodeBinary(string &opcode)
{
    if (opcode == "JMP")
        return 0;
    else if (opcode == "JRP")
        return 1;
    else if (opcode == "LDN")
        return 2;
    else if (opcode == "STO")
        return 3;
    else if (opcode == "SUB")
        return 4;
    else if (opcode == "SUB_1")
        return 5;
    else if (opcode == "CMP")
        return 6;
    else if (opcode == "STP")
        return 7;
    else if (opcode == "ADD")
        return 8;
    else if (opcode == "MTP")
        return 9;
    else if (opcode == "DIV")
        return 10;
    else if (opcode == "MOD")
        return 11;
    else if (opcode == "SQR")
        return 12;
    else if (opcode == "LDP")
        return 13;
    else if (opcode == "MOV")
        return 14;
    else if (opcode == "AND")
        return 15;
    else if (opcode == "OR")
        return 16;
    else if (opcode == "NOT")
        return 17;
    else if (opcode == "XOR")
        return 18;

    else
        return -1;
}

/**
 * This methods write machine code into a file after conversion
 * @param fileName The name of the fie that contains the machine code
 */
void Assembler::writeMachineCode(const string &fileName)
{
    ofstream ofs(fileName);
    if (!ofs)
    {
        cerr << "Error opening file for output" << endl;
        exit(EXIT_FAILURE);
    }

    for (const vector<char> &instruction : assembled)
    {
        for (char bit : instruction)
        {
            ofs << bit;
        }
        ofs << endl;
    }
    ofs.close();
}

/**
 *  This method is used to print
 * @tparam T The type
 * @param toPrint The things to print
 * @param a
 */
template <typename T>
void Assembler::printLine(vector<T> toPrint, bool a)
{
    for (int i = 0; i < toPrint.size(); i++)
    {
        cout << toPrint[i];
        if (a)
        {
            cout << " ";
        }
    }
    cout << endl;
}

/**
 * This method print the content of a 2D vector
 * @tparam T The type of contents
 * @param content the 2D vector to print
 */
template <typename T>
void Assembler::printContent(vector<vector<T>> content)
{
    cout << "Current Content: " << endl;
    for (const auto &line : content)
    {
        for (const auto &token : line)
        {
            cout << token << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Print the key-value in the map
 * @param myMap The map to print
 */
void Assembler::printMapValues(const map<string, int> &myMap)
{
    cout << "Content in Symbol Table: " << endl;
    for (const auto &pair : myMap)
    {

        cout << "Name '" << pair.first << "': " << pair.second << endl;
    }
    cout << endl;
}

// 转换int到LSB二进制数
vector<char> Assembler::decToBin(long long int integer, int size)
{

    vector<char> bin;
    int num;
    if (integer < 0)
    {
        num = -integer;
    }
    else
    {
        num = integer;
    }

    while (num != 0)
    {
        if (num % 2 == 0)
        {
            bin.push_back('0');
        }
        else
        {
            bin.push_back('1');
        }
        num = num >> 1;
    }

    while (bin.size() < size - 1)
    {
        bin.push_back('0');
    }

    if (integer < 0 && bin.size() == size - 1)
    {
        bin.push_back('1');
    }
    else if (bin.size() == size - 1)
    {
        bin.push_back('0');
    }

    return bin;
}

bool Assembler::turnInteger(string &token, int &value)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (i == 0 && token[i] == '-' && token.size() > 1)
            continue;
        if (token[i] < '0' || token[i] > '9')
        {
            cout << "Invalid Instruction: " << token << endl;
            exit(EXIT_FAILURE);
            return false;
        }
    }
    try
    {
        value = stoi(token);
    }
    catch (const invalid_argument &e)
    {
        cout << "Invalid Instruction: " << token << endl;
        exit(EXIT_FAILURE);
        return false;
    }
    catch (const out_of_range &e)
    {
        cout << "Invalid Instruction: " << token << endl;
        exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

void Assembler::printCorresponding()
{
    cout << "Transferring..." << endl;
    int lineCount = 0;
    // 可以根据需要扩展加载符号表的功能
    for (size_t i = 0; i < toAssemble.size(); ++i)
    {
        auto &line = toAssemble[i];
        cout << "Before transformation: ";
        printLine(line, true);
        cout << "After transformation: " << endl;
        printLine(assembled[lineCount], false);
        if (find(line.begin(), line.end(), "MOV") != line.end())
        {
            // Check if "MOV" is present in the line
            lineCount++;
            printLine(assembled[lineCount], false);
        }
        lineCount++;
    }
    cout << endl;
}