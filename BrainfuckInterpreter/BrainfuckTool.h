#pragma once
#include <string>

class BrainfuckTool
{
public:
	void SetValue(int value);
	void SetArray(char inputArray[], int arrayLength);
	void Right(int amount = 1);
	void Left(int amount = 1);

	std::string bfString;
	//void Plus();
	//void Minus();
};

