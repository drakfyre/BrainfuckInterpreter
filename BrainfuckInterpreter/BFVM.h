#include <iostream>
#include <string>

using std::string;

#pragma once
class BFVM
{
public:
    BFVM(string brainfuckString) : brainfuckString{ brainfuckString } {};
    bool Step(bool resolveLoops = true);

private:
    uint8_t data[30000] = { 0 };
    int dataIndex = 0;
    int currentCharacterIndex = 0;
    string brainfuckString = "";
    bool evaluatingLoop = false;

    BFVM() = default;
    void ScanForBracket(int& currentCharacter, char bracketType);

    friend class BrainfuckTool;
};
