#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "BrainfuckTool.h"
#include "BFVM.h"

using std::string;

int main()
{
    char levelArray[] = {
        '+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',
        '|','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','*','|',
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

    constexpr int kLevelWidth = 25;
    constexpr int kLevelHeight = 15;

    BrainfuckTool tool;
    int wIndex = tool.NewVariable('w');
    int aIndex = tool.NewVariable('a');
    int sIndex = tool.NewVariable('s');
    int dIndex = tool.NewVariable('d');
    int inputCharacterIndex = tool.NewVariable(0);
    int widthIndex = tool.NewVariable(kLevelWidth);
    int playerPositionIndex = tool.NewVariable(1 + kLevelWidth * 1); // Position (1,1)
    int hasKeyIndex = tool.NewVariable(0);
    int gameRunning = tool.NewVariable(1);
    int gameMapIndex = tool.NewArray(levelArray, kLevelWidth * kLevelHeight, kLevelWidth);

    tool.ChangeIndexAbsolute(gameRunning);
    tool.Branch();
    tool.ChangeIndexAbsolute(gameMapIndex);
    tool.OutString(kLevelWidth * kLevelHeight + kLevelHeight); // Level width*height + 1 per height (because of the newlines we added in NewArray)
    tool.ChangeIndexAbsolute(inputCharacterIndex);
    tool.In();
    //tool.PlayerLogic(wIndex,aIndex,sIndex,dIndex);
    tool.ChangeIndexAbsolute(gameRunning);
    tool.Loop();

    // Okay, so in our original progam, during our output function if the position matches the player, we output @
    // Doing that during the output in Brainfuck would require us to thrash around between memory, checking every spot to see if it should be @
    // Remember, distance in brainfuck is more >>> and <<< so we want to minimize jumps (this is also why I encoded \n into the array)
    // So in our new program, what we will do is actually PUT the @ symbol in the position inside the map itself
    // So that the output code can just output stuff and not jump around
    // So I guess that code will go here
    // I set up the player to have separate x and y, I feel this will be a mistake, but I need practice so this will be good practice.
    // As such, we need to multiply the y by width (which means we have to store width too) and we need to then add x, and put that in a new variable

    //std::cout << tool.bfString;

    std::ifstream program("week6alpha.b");
    std::stringstream buffer;
    buffer << program.rdbuf();


    //std::cout << "Please type in your Brainfuck program now.\n";

    //string brainfuckString = buffer.str();

    //std::cin >> brainfuckString;

    //BFVM bfvm(tool.);
    //while(bfvm.Step())
    //{

    //}
}