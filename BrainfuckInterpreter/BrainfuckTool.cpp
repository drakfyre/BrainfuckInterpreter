#include "BrainfuckTool.h"
#include <iostream>

void BrainfuckTool::SetValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		//std::cout << '+';
		bfString += '+';
	}
}

void BrainfuckTool::SetArray(char inputArray[], int arrayLength, int arrayWidth)
{
	for (int i = 0; i < arrayLength; i++)
	{
		SetValue(inputArray[i]);
		Right();

		if (i % arrayWidth == arrayWidth - 1)
		{
			SetValue('\n');
			Right();
		}
	}
}

void BrainfuckTool::Right(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		//std::cout << '>';
		bfString += '>';
	}
}

void BrainfuckTool::Left(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		//std::cout << '<';
		bfString += '<';
	}
}

void BrainfuckTool::Out(int length)
{
	for (int i = 0; i < length; i++)
	{
		bfString += '.';
		Right();
	}
}

// Copies value from current memory position to offset from current memory positon
void BrainfuckTool::CopyTo(int offset)
{
	// I've never brainfucked before so bear with me
	// We kinda can't make this non-destructive in some way, at least in interim
	// We may need to have an offset to a scratch pad area, but let's try to avoid that
	// We will currently assume that the destination contains 0, but we'll circle back around on that later
	
}
