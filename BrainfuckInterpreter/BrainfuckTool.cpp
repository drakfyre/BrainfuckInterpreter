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
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
}

void BrainfuckTool::Left()
{
	bfvm.brainfuckString += '<';
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
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
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
}

void BrainfuckTool::Minus()
{
	bfvm.brainfuckString += '-';
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
}

void BrainfuckTool::In()
{
	bfvm.brainfuckString += ',';
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
}

void BrainfuckTool::Out()
{
	bfvm.brainfuckString += '.';
	if (!bracketDepth)
	{
		bfvm.Step();
	}
	else
	{
		executionQueue++;
	}
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

void BrainfuckTool::Branch(bool waitToResolve)
{
	bfvm.brainfuckString += '[';

	if(waitToResolve)
	{
		executionQueue++;
		bracketDepth++;
	}
	else
	{
		bfvm.Step();
	}
}

void BrainfuckTool::Loop(bool resolveLoops)
{
	bfvm.brainfuckString += ']';
	executionQueue++;
	bracketDepth--;
	if(bracketDepth < 0)
	{
		bracketDepth = 0;
	}

	if (!bracketDepth)
	{
		while (executionQueue)
		{
			executionQueue--;
			bfvm.Step(resolveLoops);
		}
	}
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
	Right();
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
	ChangeIndexAbsolute(origin);
}

void BrainfuckTool::SubtractFromOffset(int offset)
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
	Minus();						// We subtract 1 from our destination
	ChangeIndexAbsolute(counter);	// We change index to our counter
	Minus();						// Subtract 1 from our counter
	Loop();							// Loop till counter is 0
	ChangeIndexAbsolute(origin);
}

void BrainfuckTool::CopyToIndex(int index)
{
	int offset = index - bfvm.dataIndex;
	CopyToOffset(offset);
}

void BrainfuckTool::AddToIndex(int index)
{
	CopyToIndex(index);
}


void BrainfuckTool::SubtractFromIndex(int index)
{
	int offset = index - bfvm.dataIndex;
	SubtractFromOffset(offset);
}

void BrainfuckTool::Not()
{
	// No matter what value in the current data slot, put a 1 next door
	Right();
	Plus();

	// Go back to check our original number
	Left();

	// This checks our original number
	Branch();
		// We're not zero if we're here
		Branch();
			Minus();
		Loop();
		// Now we're 0
		Right();
		Minus(); // 0 next door, so we skip the 0 logic ahead
		Left();  // back to origin
	Loop();

	// Go right to check our neighbor
	Right();

	// This checks our neighbor, which would be a 1 if our original number was a 0
	Branch();
		// Our original number was 0 if we're here
		Left();
		Plus();		// Make our 0 into a 1
		Right();
		Minus();	// Reset our neighbor
	Loop();
	Left();
}

void BrainfuckTool::NonDestructiveNot()
{
	// No matter what value in the current data slot, put a 1 next door
	Right();
	Plus();

	// Go back to check our original number
	Left();

	// This checks our original number
	Branch();
		// We're not zero if we're here
		Right();
		Minus(); // 0 next door
		Left();  // back to origin
	Loop();
}

void BrainfuckTool::PlayerLogic(int wIndex, int aIndex, int sIndex, int dIndex, int playerPositionIndex, int widthIndex)
{
	// Current index is the key pressed

	int origin = bfvm.dataIndex;
	int counter = NewTempVariable();

	ChangeIndexAbsolute(origin);

	Branch();
	// Add to counter
	ChangeIndexAbsolute(counter);	// ChangeIndexAbsolute breaks inside branches because we don't evaluate up to this point...
	Plus();
	// Subtract from all till origin is 0
	ChangeIndexAbsolute(wIndex);
	Minus();
	ChangeIndexAbsolute(aIndex);
	Minus();
	ChangeIndexAbsolute(sIndex);
	Minus();
	ChangeIndexAbsolute(dIndex);
	Minus();
	ChangeIndexAbsolute(origin);
	Minus();
	Loop();	// Origin is now 0, counter is now original value, and maybe one of w/a/s/d indexes are 0?

	ChangeIndexAbsolute(wIndex);
	NonDestructiveNot();
	Right();
	Branch();
		// w was entered
		Minus(); // Manually reset our nondestructivenot (may want to handle this elsewhere later)
		ChangeIndexAbsolute(widthIndex);
		SubtractFromIndex(playerPositionIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(wIndex);
		Right();
	Loop();

	ChangeIndexAbsolute(aIndex);
	NonDestructiveNot();
	Right();
	Branch();
		// a was entered
		Minus(); // Manually reset our nondestructivenot (may want to handle this elsewhere later)
		ChangeIndexAbsolute(playerPositionIndex);
		Minus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(aIndex);
		Right();
	Loop();

	ChangeIndexAbsolute(sIndex);
	NonDestructiveNot();
	Right();
	Branch();
		// s was entered
		Minus(); // Manually reset our nondestructivenot (may want to handle this elsewhere later)
		ChangeIndexAbsolute(widthIndex);
		AddToIndex(playerPositionIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(sIndex);
		Right();
	Loop();

	ChangeIndexAbsolute(dIndex);
	NonDestructiveNot();
	Right();
	Branch();
		// d was entered
		Minus(); // Manually reset our nondestructivenot (may want to handle this elsewhere later)
		ChangeIndexAbsolute(playerPositionIndex);
		Plus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(dIndex);
		Right();
	Loop();

	ChangeIndexAbsolute(counter);

	Branch();
	// Add to all till origin is original number
	ChangeIndexAbsolute(wIndex);
	Plus();
	ChangeIndexAbsolute(aIndex);
	Plus();
	ChangeIndexAbsolute(sIndex);
	Plus();
	ChangeIndexAbsolute(dIndex);
	Plus();
	ChangeIndexAbsolute(origin);
	Plus();
	// Subtract from counter
	ChangeIndexAbsolute(counter);
	Minus();
	Loop();	// Origin is now original value, as are all the other indexes, counter is now 0

	ChangeIndexAbsolute(origin);
}