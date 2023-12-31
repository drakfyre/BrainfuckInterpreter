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
	void In();
	void Out();
	void Branch();
	void Loop(bool resolve = true);

	/// Macro functionality
	// This currently assumes that the value we are setting is 0
	void SetValue(int value);
	void SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);
	int NewVariable(int value);
	int NewArray(char inputArray[], int arrayLength, int arrayWidth);
	void ChangeIndexRelative(int offset);
	void ChangeIndexAbsolute(int index);
	void ChangeIndexToNextZero();
	void CopyToOffset(int offset);
	void CopyToIndex(int index);
	void PlayerLogic(int wIndex, int aIndex, int sIndex, int dIndex);
	void MoveToOffset(int offset);
	void MoveToIndex(int index); // might have a problem?
	void OutString(int length);

	BFVM bfvm;

private:
	int NewTempVariable();
};

