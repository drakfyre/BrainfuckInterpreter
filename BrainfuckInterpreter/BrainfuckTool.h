#pragma once
#include <string>

class BrainfuckTool
{
public:
	void SetValue(int value);
	void SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);
	void Right(int amount = 1);
	void Left(int amount = 1);
	void Add();
	void Minus();
	void Out(int length = 1);
	void Branch();
	void Loop();
	void CopyTo(int offset);

	std::string bfString = "";
	//void Plus();
	//void Minus();
};

