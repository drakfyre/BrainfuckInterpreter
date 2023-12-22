// BrainfuckInterpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "BrainfuckTool.h"

using std::string;

void ScanForBracket(char*& currentCharacter, char bracketType);

int main()
{
    char levelArray[] = { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','*','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','+','-',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',
                      '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ',' ','X','|',
                      '+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+' };

    BrainfuckTool tool;
    tool.SetArray(levelArray, 25*15);


    std::ifstream program("sierpinski.b");
    std::stringstream buffer;
    buffer << program.rdbuf();


    //std::cout << "Please type in your Brainfuck program now.\n";

    string brainfuckString = buffer.str();

    //std::cin >> brainfuckString;

    uint8_t data[30000] = { 0 };
    uint8_t* dataPointer = data;

    char* currentCharacter = &brainfuckString[0];


    while (*currentCharacter != '\0')
    {
        switch (*currentCharacter)
        {
            case '>':
                // Increment the data pointer by one (to point to the next cell to the right).
                dataPointer++;
                if (dataPointer - data >= 30000)
                {
                    dataPointer = data;
                }
                break;
            case '<':
                // Decrement the data pointer by one (to point to the next cell to the left).
                dataPointer--;
                if(dataPointer - data < 0)
                {
                    dataPointer = data + 30000 - 1;
                }
                break;
            case '+':
                // Increment the byte at the data pointer by one.
                (*dataPointer)++;
                break;
            case '-':
                // Decrement the byte at the data pointer by one.
                (*dataPointer)--;
                break;
            case '.':
                // 	Output the byte at the data pointer.
                std::cout << *dataPointer;
                break;
            case ',':
                // Accept one byte of input, storing its value in the byte at the data pointer.
                std::cin >> *dataPointer;
                break;
            case '[':
                // 	If the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
                if (*dataPointer == 0)
                {
                    // Scan forward and set data pointer to matching ]
                    ScanForBracket(currentCharacter, '[');
                }
                break;
            case ']':
                // If the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.
                if (*dataPointer != 0)
                {
                    // Scan backward and set data pointer to matching [
                    ScanForBracket(currentCharacter, ']');
                }
                break;
            default:
                break;
        }
        currentCharacter++;
    }
}

// This scans for the corresponding closing/opening bracket 
void ScanForBracket(char*& currentCharacter, char bracketType)
{
    int bracketCount = 0;
    char otherBracket;
    bool scanBackwards = false;

    if(bracketType == '[')
    {
        otherBracket = ']';
    }
    else
    {
        otherBracket = '[';
        scanBackwards = true;
    }

    while(true)
    {
        if(scanBackwards)
        {
            currentCharacter--;
        }
        else
        {
            currentCharacter++;
        }

        if (*currentCharacter == bracketType)
        {
            bracketCount++;
        }
        else if (*currentCharacter == otherBracket)
        {
            if (bracketCount > 0)
            {
                bracketCount--;
            }
            else
            {
                // We are already at the correct bracket, so we just have to return
                return;
            }
        }
    }
}