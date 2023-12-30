#include "BFVM.h"
#include <iostream>
#include <string>
#include "BrainfuckTool.h"

// Steps the VM, if we run out of instructions we return false
bool BFVM::Step()
{
    if(brainfuckString[currentCharacterIndex] != '\0')
    {
        switch (brainfuckString[currentCharacterIndex])
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
            if (dataPointer - data < 0)
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
                ScanForBracket(currentCharacterIndex, '[');
            }
            break;
        case ']':
            // If the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.
            if (*dataPointer != 0)
            {
                // Scan backward and set data pointer to matching [
                ScanForBracket(currentCharacterIndex, ']');
            }
            break;
        default:
            break;
        }
        currentCharacterIndex++;
        return true;
    }

    std::cout << "Final command pointer position: " << currentCharacterIndex;
    return false;
}

// This scans for the corresponding closing/opening bracket 
void BFVM::ScanForBracket(int& currentCharacter, char bracketType)
{
    int bracketCount = 0;
    char otherBracket;
    bool scanBackwards = false;

    if (bracketType == '[')
    {
        otherBracket = ']';
    }
    else
    {
        otherBracket = '[';
        scanBackwards = true;
    }

    while (true)
    {
        if (scanBackwards)
        {
            currentCharacter--;
        }
        else
        {
            currentCharacter++;
        }

        if (brainfuckString[currentCharacterIndex] == bracketType)
        {
            bracketCount++;
        }
        else if (brainfuckString[currentCharacterIndex] == otherBracket)
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