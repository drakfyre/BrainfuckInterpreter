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
