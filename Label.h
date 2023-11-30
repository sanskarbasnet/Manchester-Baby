#ifndef LABEL_H
#define LABEL_H

#include <iostream>
#include <string>
using namespace std;

// Declaration of the Label class
class Label
{
private:
    string symbol;      // Contains the symbol of the label
    string address;     // Contains the address
    int currentLine;    // Contains the line where the label is declared
    int size;           // Declares the size of the label

public:
    // Constructor for the Label class
    Label(string symb, int line, int s);

    // Destructor for the Label class
    ~Label();

    // Converts int to a string of binary
    void convertBinary();

    // Setter for the symbol of the label
    void setSymbol(string symb);

    // Setter for the line where the label is declared
    void setCurrentLine(int line);

    // Setter for the size of the label
    void setSize(int s);

    // Setter for the address of the label
    void setAddress(string address);

    // Getter for the symbol of the label
    string getSymbol();

    // Getter for the line where the label is declared
    int getCurrentLine();

    // Getter for the size of the label
    int getSize();

    // Getter for the address of the label
    string getAddress();
};

#endif