#include <iostream>
#include <string>

using std::string;

#pragma once
class BFVM
{
private:
    uint8_t data[30000] = { 0 };
    uint8_t* dataPointer = data;
    char* currentCharacter;
    string brainfuckString = "";

public:
    BFVM(string brainfuckString) : brainfuckString{ brainfuckString } { currentCharacter = &this->brainfuckString[0]; };
    bool Step();
    void ScanForBracket(char*& currentCharacter, char bracketType);
};
