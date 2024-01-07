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

	/// Pure brainfuck
	void SetZero();
	void ChangeIndexToNextTempZero();
	void ChangeIndexToPreviousTempZero();
	void ChangeIndexToPreviousTempOne();
	void Not();

	/// Macro value functions (affects current data slot with immediate values)
	void AddValue(int value);
	void SubtractValue(int value);
	void SetValue(int value);
	void SetArray(char inputArray[], int arrayLength, int arrayWidth = 0);

	/// Macro offset functions (affects offset data slot with current data slot values)
	void MoveToOffset(int offset);
	void AddToOffset(int offset);
	void SubtractFromOffset(int offset);
	void CopyToOffset(int offset);

	/// Macro output functions
	void OutString(int length);

	/// Macro variable tracking functions (returns index of data, these should be set before any other commands)
	int NewVariable(int value);
	int NewArray(char inputArray[], int arrayLength, int arrayWidth);

	/// Macro data index modification functions
	void ChangeIndexRelative(int offset);
	void ChangeIndexRelativeToValueAtOffset(int offset);

	/// Game logic that probably shouldn't be here
	void PlayerLogic(int inputCharacterIndex, int wIndex, int aIndex, int sIndex, int dIndex, int wIndexTemp, int aIndexTemp, int sIndexTemp, int dIndexTemp, int playerPositionIndex, int playerPositionIndexTemp, int levelIndex, int widthIndex);

	BFVM bfvm;

private:
	void NewTempVariable();
	int bracketDepth = 0;
	int executionQueue = 0;
	int virtualDataIndex = 0;
};

