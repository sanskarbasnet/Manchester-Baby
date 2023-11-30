#include <iostream>
#include <string>
#include <vector>
#include "Label.h"

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;

// Declaration of the Assembler class
class Assembler
{
private:
    string fileName;               // Contains the name of the source code file
    int currentLineNum;            // Represents the current line number being processed
    int totalLines;                // Represents the total number of lines in the source code
    vector<Label *> symbolTable;   // Stores labels with their symbols, addresses, line numbers, and sizes
    vector<string *> tempStorage;  // Stores temporary machine code lines during assembly

public:
    // Constructor for the Assembler class, initializes member variables to default values
    Assembler();

    // Destructor for the Assembler class, frees memory and resets member variables
    ~Assembler();

    // Setter for the source code file name
    void setfileName(string fileName);

    // Setter for the total number of lines in the source code
    void setTotalLines(int lineNum);

    // Loads the source code file, performs two passes of decoding, and saves the machine code
    void loadFile();

    // Decodes the current line during the specified pass
    void decodeLine(int pass, string line);

    // Saves the generated machine code to an output file named "output.txt"
    void saveFile();

    // Gets the machine code opcode for the given instruction
    string getOpCode(string instruction);

    // Converts a decimal number to a binary string of a specified bit size
    string convertBinary(int dec, int bit);
};

#endif