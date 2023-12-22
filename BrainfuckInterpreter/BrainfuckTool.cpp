#include "BrainfuckTool.h"
#include <iostream>

void BrainfuckTool::SetValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		std::cout << '+';
	}
}

void BrainfuckTool::SetArray(char inputArray[], int arrayLength)
{
	for (int i = 0; i < arrayLength; i++)
	{
		SetValue(inputArray[i]);
		Right();
	}
}

void BrainfuckTool::Right(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		std::cout << '>';
	}
}

void BrainfuckTool::Left(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		std::cout << '<';
	}
}
