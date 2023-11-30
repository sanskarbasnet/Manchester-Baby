#include <iostream>
#include <string>
#include <algorithm>
#include "Label.h"

using namespace std;

// Constructor for the Label class, initializes symbol, currentLine, size, and converts currentLine to binary.
Label::Label(string symb, int line, int s)
{
    symbol = symb;
    currentLine = line;
    size = s;
    convertBinary();
}

// Destructor for the Label class, resets member variables.
Label::~Label()
{
    symbol = "";
    currentLine = 0;
    size = 0;
}

// Converts the currentLine value to a binary string and stores it in the address member variable.
void Label::convertBinary()
{
    int dec = currentLine;
    int bit = 13;
    string bin = "";

    // Ensure decTemp is positive for calculation
    int decTemp = (dec < 0) ? -dec : dec;

    // Convert decimal to binary
    while (decTemp != 0)
    {
        bin += (decTemp % 2 == 0) ? '0' : '1';
        decTemp /= 2;
    }

    int binSize = bin.length();

    // Ensure binary string is of the specified bit size
    if (binSize < bit - 1)
    {
        bin.append(bit - 1 - binSize, '0');
    }

    // Add sign bit (1 for negative, 0 for non-negative)
    address = (dec < 0) ? bin + '1' : bin + '0';
}

// Setter for the symbol of the label.
void Label::setSymbol(string symb)
{
    symbol = symb;
}

// Setter for the line where the label is declared.
void Label::setCurrentLine(int line)
{
    currentLine = line;
}

// Setter for the size of the label.
void Label::setSize(int s)
{
    size = s;
}

// Setter for the address of the label.
void Label::setAddress(string addr)
{
    address = addr;
}

// Getter for the symbol of the label.
string Label::getSymbol()
{
    return symbol;
}

// Getter for the line where the label is declared.
int Label::getCurrentLine()
{
    return currentLine;
}

// Getter for the size of the label.
int Label::getSize()
{
    return size;
}

// Getter for the address of the label.
string Label::getAddress()
{
    return address;
}