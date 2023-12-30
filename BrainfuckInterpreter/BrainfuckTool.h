#pragma once
#include <string>
#include "BFVM.h"

class BrainfuckTool
{
public:
	// This currently assumes that the value we are setting is 0
	void SetValue(int value);
	void SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);
	void Right(int amount = 1);
	void Left(int amount = 1);
	void ChangeIndexRelative(int offset);
	void Plus();
	void Minus();
	void Out(int length = 1);
	void Branch();
	void Loop();
	void CopyTo(int offset);
	void MoveToOffset(int offset);
	void MoveToIndex(int index);

	BFVM bfvm;
};

