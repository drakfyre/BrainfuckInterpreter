// BrainfuckInterpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using std::string;

void ScanForBracket(char*& currentCharacter, char bracketType);

int main()
{
    std::cout << "Please type in your Brainfuck program now.\n";

    string brainfuckString;
    std::cin >> brainfuckString;

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
                currentCharacter++;
                break;
            case '<':
                // Decrement the data pointer by one (to point to the next cell to the left).
                dataPointer--;
                currentCharacter++;
                break;
            case '+':
                // Increment the byte at the data pointer by one.
                (*dataPointer)++;
                currentCharacter++;
                break;
            case '-':
                // Decrement the byte at the data pointer by one.
                (*dataPointer)--;
                currentCharacter++;
                break;
            case '.':
                // 	Output the byte at the data pointer.
                std::cout << *dataPointer;
                currentCharacter++;
                break;
            case ',':
                // Accept one byte of input, storing its value in the byte at the data pointer.
                std::cin >> dataPointer;
                currentCharacter++;
                break;
            case '[':
                // 	If the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
                if (*dataPointer == 0)
                {
                    // Scan forward and set data pointer to matching ]
                    ScanForBracket(currentCharacter, '[');
                }
                else
                {
                    currentCharacter++;
                }
                break;
            case ']':
                // If the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.
                if (*dataPointer != 0)
                {
                    // Scan backward and set data pointer to matching [
                    ScanForBracket(currentCharacter, ']');
                }
                else
                {
                    currentCharacter++;
                }
                break;
            default:
                break;
        }
    }
}

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
                // We are already at the right bracket, but we want to be the command before/after it
                if (scanBackwards)
                {
                    currentCharacter--;
                }
                else
                {
                    currentCharacter++;
                }
                return;
            }
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
