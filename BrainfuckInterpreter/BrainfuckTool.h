#pragma once
#include <string>
#include "BFVM.h"

class BrainfuckTool
{
public:
	/// Built-in brainfuck
	void Right();
	void Left();
	void Plus();
	void Minus();
	// In
	void Out(int length = 1);
	void Branch();
	void Loop();

	/// Our additional functionality
	// This currently assumes that the value we are setting is 0
	int SetValue(int value);
	int SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);
	void ChangeIndexRelative(int offset);
	void ChangeIndexToNextZero();
	void CopyTo(int offset);
	void MoveToOffset(int offset);
	void MoveToIndex(int index);

	BFVM bfvm;
};

