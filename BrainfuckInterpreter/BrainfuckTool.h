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
	void Branch(bool waitToResolve = true);
	void Loop(bool resolve = true);

	/// Macro functionality
	// This currently assumes that the value we are setting is 0
	void SetValue(int value);
	void AddValue(int value);
	void SubtractValue(int value);
	void SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);
	int NewVariable(int value);
	int NewArray(char inputArray[], int arrayLength, int arrayWidth);
	void ChangeIndexRelative(int offset);
	void ChangeIndexAbsolute(int index);
	void ChangeIndexRelativeToValueAtIndex(int index);
	void ChangeIndexLeftRelativeToValueAtIndex(int index);
	void ScanLeftForOne();
	void ChangeIndexToNextZero();
	void ChangeIndexToNextTempZero();
	void ChangeIndexToPreviousTempZero();
	void ChangeIndexToPreviousTempOne();
	void CopyToOffset(int offset);
	void SubtractFromOffset(int offset);
	void CopyToIndex(int index);
	void AddToOffset(int offset);
	void AddToIndex(int index);
	void SubtractFromIndex(int index);
	void SetZero();
	void Not();
	void NotInMyBackyard(int tempVariableIndex);
	void PlayerLogic(int wIndex, int aIndex, int sIndex, int dIndex, int wIndexTemp, int aIndexTemp, int sIndexTemp, int dIndexTemp, int playerPositionIndex, int playerPositionIndexTemp, int levelIndex, int widthIndex);
	void MoveToOffset(int offset);
	void MoveToIndex(int index); // might have a problem?
	void OutString(int length);
	int GetVirtualDataIndex() { return virtualDataIndex; }

	BFVM bfvm;

private:
	int NewTempVariable();
	int bracketDepth = 0;
	int executionQueue = 0;
	int virtualDataIndex = 0;
};

