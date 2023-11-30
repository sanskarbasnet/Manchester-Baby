#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "Assembler.h"
#include "Label.h"

using namespace std;

// Main function prompting the user to enter the file name of the source code.
int main()
{
    cout << "#############################################" << endl;
    cout << "#    Welcome to Manchester Baby Assembler   #" << endl;
    cout << "#############################################" << endl;
    cout << "Please enter the file name of the source code:" << endl;
    string answer;
    getline(cin, answer);

// Create an Assembler object and set the source code file name.
    Assembler *assembler = new Assembler();
    assembler->setfileName(answer);

    cout << "Output will be saved as output.txt" << endl;
 // Load the source code, process it in two passes, and generate machine code.
    assembler->loadFile();
    delete assembler; // Deallocate memory.
}
// Constructor for the Assembler class, initializing variables to default values.
Assembler::Assembler()
{
    fileName = "original.txt";
    currentLineNum = 0;
    totalLines = 0;
}
// Destructor for the Assembler class, freeing allocated memory.
Assembler::~Assembler()
{
    fileName.clear();
    currentLineNum = 0;
    totalLines = 0;
    for (int i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable.at(i) != NULL)
        {
            delete symbolTable.at(i);
            symbolTable.at(i) = NULL;
        }
    }
    symbolTable.clear();
    for (int i = 0; i < tempStorage.size(); i++)
    {
        if (tempStorage.at(i) != NULL)
        {
            delete tempStorage.at(i);
            tempStorage.at(i) = NULL;
        }
    }
    tempStorage.clear();
    symbolTable.clear();
};

// Setter for the file name.
void Assembler::setfileName(string file)
{
    fileName = file;
}

// Setter for the total number of lines in the source code.
void Assembler::setTotalLines(int lineNum)
{
    totalLines = lineNum;
}

// Load the source code file, process it in two passes, and generate machine code.
void Assembler::loadFile()
{
    string line;

    for (int i = 0; i < 2; i++)
    {
        ifstream source(fileName.c_str());
        if (source.is_open())
        {
            while (getline(source, line))
            {
                decodeLine(i + 1, line); // i = number of passes (1st or second)
            }
            source.close();
            currentLineNum = 0;
        }
    }
    saveFile();
}

// Decode the current line during the specified pass.
void Assembler::decodeLine(int pass, string line)
{
    char first = line[0];
    bool label = false;
    int labellength = 0;

    // Check if the line is not a comment
    if (first != ';')
    { 
        // First pass
        if (pass == 1)
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end()); // Remove spaces from the line

            // Check for labels
            for (int i = 0; i < line.length(); i++)
            {
                char letter = line[i];
                if (letter == ':')
                {
                    string tempLabel;
                    labellength = i + 1;
                    tempLabel = line.substr(0, i);
                    Label *newLabel = new Label(tempLabel, currentLineNum, i);
                    if (newLabel == NULL)
                    {
                        cout << "Memory Allocation failed." << endl;
                        exit(1);
                    }
                    if (symbolTable.size() <= currentLineNum)
                    {
                        symbolTable.resize(currentLineNum + 1);
                    }
                    symbolTable[currentLineNum] = newLabel; // Add label to SymbolTable
                    label = true;
                    break;
                }
            }

            string opcode = "";
            string operand = "";

            // If there was no label
            if (!label)
            {
                opcode = line.substr(0, 3);
                for (int i = 3; i < line.length(); i++)
                {
                    if (line[i] != ';')
                    {
                        operand += line[i];
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if (label)
            {
                opcode = line.substr(labellength, 3);
                for (int i = labellength + 3; i < line.length(); i++)
                {
                    if (line[i] != ';')
                    {
                        operand += line[i];
                    }
                    else
                    {
                        break;
                    }
                }
            }

            string tempstoreline;

            // Handle special cases for opcode "STP" and "VAR"
            if (opcode == "STP")
            {
                opcode = getOpCode(opcode);
                operand = "0";
                operand = convertBinary(atoi(operand.c_str()), 13);
                tempstoreline = operand + opcode;
                tempstoreline.append(16, '0');
            }
            else if (opcode == "VAR")
            {
                operand = convertBinary(atoi(operand.c_str()), 32);
                tempstoreline = operand;
            }
            else
            {
                opcode = getOpCode(opcode);

                // If the operand is a variable
                if (isalpha(operand[0]))
                {
                    tempstoreline = operand + "|"; // Used to easily distinguish the end of a variable later on
                    tempstoreline = tempstoreline + opcode;
                    tempstoreline.append(16, '0');
                }
                else
                {
                    operand = convertBinary(atoi(operand.c_str()), 13);
                    tempstoreline = operand + opcode;
                    tempstoreline.append(16, '0');
                }
            }

            string *permstoreline = new string(tempstoreline); // Create a new string with the store line inside
            if (tempStorage.size() <= currentLineNum)
            {
                tempStorage.resize(currentLineNum + 1);
            }
            tempStorage[currentLineNum] = permstoreline; // Add it to our temp storage
        }
        // Second pass
        else
        {
            string address = "";
            string *tempLine;
            string var;

            // Iterate through the tempStorage to replace variables with addresses
            for (int i = 0; i < tempStorage.size(); i++)
            {
                tempLine = tempStorage[i];
                if (tempLine != NULL)
                {
                    string defstring = tempStorage[i]->c_str(); // Get string from pointer
                    char letter = defstring[0];

                    if (isalpha(letter))
                    { // If the first letter is a variable, change it to an address
                        var = defstring.substr(0, defstring.find("|")); // Used to find where the variable ends

                        // Search for the variable in the symbolTable
                        for (int j = 0; j < symbolTable.size(); j++)
                        {
                            if (symbolTable.at(j) != NULL)
                            {
                                if (symbolTable.at(j)->getSymbol() == var)
                                {
                                    // Get the opcode from the current version of defstring
                                    string tempopcode = defstring.substr(defstring.find("|") + 1, 3);
                                    // Rewrite defstring with the address
                                    cout << "Var: " << var << " Converted to : " << symbolTable.at(j)->getAddress() << endl;
                                    defstring = (symbolTable.at(j)->getAddress()) + tempopcode;
                                    defstring.append(16, '0');
                                    // Break out of the loop for better readability
                                    goto finish;
                                }
                            }
                        }
                    finish:
                        delete (tempStorage[i]);
                        string *defstringPt = new string(defstring);
                        if (defstringPt == NULL)
                        {
                            cout << "Memory Allocation ERROR." << endl;
                            exit(1);
                        }
                        tempStorage[i] = defstringPt;
                    }
                }
            }
        }

        cout << "Line: " << currentLineNum << " Pass: " << pass << endl; // Print current location
        currentLineNum++;
    }
}

// Save the generated machine code to an output file named "output.txt".
void Assembler::saveFile()
{
    // Check if there is any machine code to save
    if (tempStorage.size() == 0)
    {
        cout << "File to load did not exist!" << endl;
        exit(1);
    }

    string output = "output.txt";
    ofstream source(output.c_str());

    // Check if the output file is open for writing
    if (source.is_open())
    {
        // Write each line of machine code to the output file
        for (int i = 0; i < tempStorage.size(); i++)
        {
            if (tempStorage[i] != NULL)
            {
                source << *tempStorage[i] << "\n";
            }
        }
        source.close();
        cout << "Assembly Successful" << endl;
    }
}

// Get the machine code opcode for the given instruction.
string Assembler::getOpCode(string instruction)
{
    // Map instruction mnemonics to their corresponding machine code opcodes
    if (instruction == "JMP")
    {
        return "000";
    }
    else if (instruction == "JRP")
    {
        return "100";
    }
    else if (instruction == "LDN")
    {
        return "010";
    }
    else if (instruction == "STO")
    {
        return "110";
    }
    else if (instruction == "SUB")
    {
        return "001";
    }
    else if (instruction == "CMP")
    {
        return "011";
    }
    else if (instruction == "STP")
    {
        return "111";
    }
    else
    {
        return ""; // Return an empty string for unsupported instructions
    }
}

string Assembler::convertBinary(int dec,int bit){
	string bin = "";	//binary value
	int rem;			//remainder
	int binSize; 		//size of bin value 
	int decTemp = dec;		//temp decimal value


	//make decimal positive for calculation
	if(dec < 0){
		decTemp = -decTemp;
	}

	while(decTemp != 0)
	{
		rem = decTemp%2;
		decTemp = decTemp/2;
		if(rem == 0){
			bin = bin + '0';
		}
		else{
			bin = bin + '1';
		}
	}

	binSize = bin.length();
	if(binSize < bit-1){
		bin.append((bit-1) - (binSize),'0');
	}

	if(dec < 0){
		bin = bin + '1';
	}
	else{
		bin = bin + '0';
	}
	return bin;

}