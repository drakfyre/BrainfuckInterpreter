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
    constexpr int kLevelWidthPlusOne = kLevelWidth + 1;
    constexpr int kLevelHeight = 15;

    BrainfuckTool tool;
 
    int wIndex = tool.NewVariable('w');
    int aIndex = tool.NewVariable('a');
    int sIndex = tool.NewVariable('s');
    int dIndex = tool.NewVariable('d');
    int wIndexTemp = tool.NewVariable(0);
    int aIndexTemp = tool.NewVariable(0);
    int sIndexTemp = tool.NewVariable(0);
    int dIndexTemp = tool.NewVariable(0);
    int inputCharacterIndex = tool.NewVariable(0);
    int widthIndex = tool.NewVariable(kLevelWidthPlusOne);
    int playerPositionIndex = tool.NewVariable(1 + kLevelWidthPlusOne * 1); // Position (1,1)
    int playerPositionIndexTemp = tool.NewVariable(0);
    int hasKeyIndex = tool.NewVariable(0);
    int gameRunningIndex = tool.NewVariable(1);
    int gameMapIndex = tool.NewArray(levelArray, kLevelWidth * kLevelHeight, kLevelWidth);
    int oneAfterGameMapIndex = tool.NewVariable(0);

    tool.ChangeIndexRelative(gameRunningIndex - oneAfterGameMapIndex - 2);
    tool.Branch(false); // False forces immediate resolve of this branch (Can only be done with branches that will not be skipped 1st time or we'll crash)
        tool.ChangeIndexRelative(gameMapIndex - gameRunningIndex);
        tool.OutString(kLevelWidth * kLevelHeight + kLevelHeight); // Level width*height + 1 per height (because of the newlines we added in NewArray)
        tool.ChangeIndexRelative(inputCharacterIndex - (oneAfterGameMapIndex));
        tool.In();
        tool.PlayerLogic(inputCharacterIndex, wIndex, aIndex, sIndex, dIndex, wIndexTemp, aIndexTemp, sIndexTemp, dIndexTemp, playerPositionIndex, playerPositionIndexTemp, gameMapIndex, widthIndex);
        tool.ChangeIndexRelative(gameRunningIndex - inputCharacterIndex);
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


    std::ifstream program("week6beta.b");
    std::stringstream buffer;
    buffer << program.rdbuf();

    string brainfuckString = buffer.str();

    //BFVM vm(brainfuckString);
    //while (vm.Step(false));


    //std::cout << "Please type in your Brainfuck program now.\n";

    //string brainfuckString = buffer.str();

    //std::cin >> brainfuckString;

    //BFVM bfvm(tool.);
    //while(bfvm.Step())
    //{

    //}
}