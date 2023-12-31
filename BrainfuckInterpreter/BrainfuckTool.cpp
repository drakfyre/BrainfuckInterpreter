#include "BrainfuckTool.h"
#include <iostream>

// This currently assumes that the value we are setting is 0
void BrainfuckTool::SetValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		BrainfuckTool::Plus();
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

// One of the "memory aware" functions, this will set the current variable to a value and move over 2 in prep for more variable declarations
int BrainfuckTool::NewVariable(int value)
{
	int returnVal = bfvm.dataIndex;
	SetValue(value);
	Right();
	Right();
	return returnVal;
}


// We're only going to use this for our map, so it doesn't prep for more variables; gonna put the map at the end
int BrainfuckTool::NewArray(char inputArray[], int arrayLength, int arrayWidth)
{
	int returnVal = bfvm.dataIndex;
	SetArray(inputArray, arrayLength, arrayWidth);
	return returnVal;
}

void BrainfuckTool::Right()
{
	bfvm.brainfuckString += '>';
	bfvm.Step();
}

void BrainfuckTool::Left()
{
	bfvm.brainfuckString += '<';
	bfvm.Step();
}

void BrainfuckTool::ChangeIndexRelative(int offset)
{
	while(offset > 0)
	{
		Right();
		offset--;
	}
	while(offset < 0)
	{
		Left();
		offset++;
	}
}

void BrainfuckTool::ChangeIndexAbsolute(int index)
{
	int offset = index - bfvm.dataIndex;
	ChangeIndexRelative(offset);
}

// Untested, may not be totally useful
void BrainfuckTool::ChangeIndexToNextZero()
{
	Branch();	// Ends if we're currently at a 0
	Right();	// Increase current index
	Loop();		// If current index is 0, ends
}

void BrainfuckTool::Plus()
{
	bfvm.brainfuckString += '+';
	bfvm.Step();
}

void BrainfuckTool::Minus()
{
	bfvm.brainfuckString += '-';
	bfvm.Step();
}

void BrainfuckTool::Out()
{
	bfvm.brainfuckString += '.';
	bfvm.Step();
}

void BrainfuckTool::OutString(int length)
{
	while (length > 0)
	{
		Out();
		Right();
		length--;
	}
}

void BrainfuckTool::Branch()
{
	bfvm.brainfuckString += '[';
	bfvm.Step();
}

void BrainfuckTool::Loop()
{
	bfvm.brainfuckString += ']';
	bfvm.Step();
}

// Moves value from current memory position to offset from current memory position, assuming that the destination contains 0
// Does this mean that this could be generally considered "AddTo"? for the cases that are non-zero? (But that doesn't imply the move...)
// Not sure how useful this actually is, just happened to write it by accident when trying to make a more useful function
// (Lightly tested)
void BrainfuckTool::MoveToOffset(int offset)
{
	Branch();						// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
	ChangeIndexRelative(offset);	// We move to our destination
	Plus();							// We add 1 to our destination
	ChangeIndexRelative(-offset);	// We move back to our source
	Minus();						// We subtract 1 from our source
	Loop();							// If our source is 0, stop looping
}

// Untested
void BrainfuckTool::MoveToIndex(int index)
{
	int offset = index - bfvm.dataIndex;
	MoveToOffset(offset);
}

int BrainfuckTool::NewTempVariable()
{
	ChangeIndexToNextZero();
	return bfvm.dataIndex;
}

// Copies value from current memory position to offset from current memory positon, assuming that the destination contains 0
// (CURRENTLY UNTESTED)
void BrainfuckTool::CopyToOffset(int offset)
{
	int origin = bfvm.dataIndex;

	// First: Move origin to our counter temporary variable
	int counter = NewTempVariable();
	ChangeIndexAbsolute(origin);
	MoveToIndex(counter);

	Branch();						// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
	ChangeIndexAbsolute(origin);	// We change index to our origin (which now contains 0)
	Plus();							// We add 1 to our origin
	ChangeIndexRelative(offset);	// We change index to our destination
	Plus();							// We add 1 our destination
	ChangeIndexAbsolute(counter);	// We change index to our counter
	Minus();						// Subtract 1 from our counter
	Loop();							// Loop till counter is 0
}

void BrainfuckTool::CopyToIndex(int index)
{
	int offset = index - bfvm.dataIndex;
	CopyToOffset(offset);
}