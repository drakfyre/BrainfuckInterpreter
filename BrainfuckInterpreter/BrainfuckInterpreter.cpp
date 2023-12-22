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

    // Put level array into memory (at position 0) with newlines built-in based on width (I think this will be easier to work with in the long-run)
    tool.SetArray(levelArray, 25*15, 25);

    // Set our starting player position into memory (at position 25*15+15)
    // Using 3 and 5 because they are prime numbers greater than 2 so we should know when we're off by a bit
    tool.SetValue(3);
    tool.Right();       // 25*15 + 15 + 1
    tool.SetValue(5);
    tool.Right();       // 25*15 + 15 + 2

    // Okay, so in our original progam, during our output function if the position matches the player, we output @
    // Doing that during the output in Brainfuck would require us to thrash around between memory, checking every spot to see if it should be @
    // Remember, distance in brainfuck is more >>> and <<< so we want to minimize jumps (this is also why I encoded \n into the array)
    // So in our new program, what we will do is actually PUT the @ symbol in the position inside the map itself
    // So that the output code can just output stuff and not jump around
    // So I guess that code will go here
    // I set up the player to have separate x and y, I feel this will be a mistake, but I need practice so this will be good practice.
    // As such, we need to multiply the y by width (which means we have to store width too) and we need to then add x, and put that in a new variable

    // Set width constant (we may move this around later)
    tool.SetValue(25);
    tool.Right();       // 25*15 + 15 + 3


    // Move back to position 0 (move left size of the array, it's 25*15 + 15 characters because of the newlines we stuffed in the SetArray function)
    tool.Left(25*15 + 15 + 3);

    // Finally, we output the whole thing
    tool.Out(25*15 + 15);

    //std::cout << tool.bfString;

    std::ifstream program("week6alpha.b");
    std::stringstream buffer;
    buffer << program.rdbuf();


    //std::cout << "Please type in your Brainfuck program now.\n";

    //string brainfuckString = buffer.str();
    string brainfuckString = tool.bfString;

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