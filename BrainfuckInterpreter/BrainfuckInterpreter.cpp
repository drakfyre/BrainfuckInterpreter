// BrainfuckInterpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using std::string;

int main()
{
    std::cout << "Please type in your Brainfuck program now.\n";

    string brainfuckString;
    std::cin >> brainfuckString;

    char* currentCharacter = &brainfuckString[0];
    while (currentCharacter != '\0')
    {
        switch (*currentCharacter)
        {
            case '>':
                // Increment the data pointer by one (to point to the next cell to the right).
                break;
            case '<':
                // Decrement the data pointer by one (to point to the next cell to the left).
                break;
            case '+':
                // Increment the byte at the data pointer by one.
                break;
            case '-':
                // Decrement the byte at the data pointer by one.
                break;
            case '.':
                // 	Output the byte at the data pointer.
                break;
            case ',':
                // Accept one byte of input, storing its value in the byte at the data pointer.
                break;
            case '[':
                // 	If the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
                break;
            case ']':
                // If the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.
                break;
            default:
                break;
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
