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
		Right();

		if (i % arrayWidth == arrayWidth - 1)
		{
			SetValue('\n');
			Right();
			Right();
		}
	}
}

// One of the "memory aware" functions, this will set the current variable to a value and move over 2 in prep for more variable declarations
int BrainfuckTool::NewVariable(int value)
{
	int returnVal = virtualDataIndex;
	SetValue(value);
	Right();
	Right();
	return returnVal;
}


// We're only going to use this for our map, so it doesn't prep for more variables; gonna put the map at the end
int BrainfuckTool::NewArray(char inputArray[], int arrayLength, int arrayWidth)
{
	int returnVal = virtualDataIndex;
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
	virtualDataIndex++;
	if(virtualDataIndex >= 30000)
	{
		virtualDataIndex = 0;
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
	virtualDataIndex--;
	if (virtualDataIndex < 0)
	{
		virtualDataIndex = 30000 - 1;
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
	int offset = index - virtualDataIndex;
	ChangeIndexRelative(offset);
}

// Untested, may not be totally useful
void BrainfuckTool::ChangeIndexToNextZero()
{
	Branch();	// Ends if we're currently at a 0
		Right();	// Increase current index
	Loop();		// If current index is 0, ends
}

void BrainfuckTool::ChangeIndexToNextTempZero()
{
	Branch();	// Ends if we're currently at a 0
		Right();	// Increase current index
		Right();	// Increase current index
	Loop();		// If current index is 0, ends
}

void BrainfuckTool::ChangeIndexToPreviousTempZero()
{
	Branch();	// Ends if we're currently at a 0
		Left();	// Decrease current index
		Left();	// Decrease current index
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
		virtualDataIndex = bfvm.dataIndex; // HACK: This is likely a massive hack; let's see how it goes
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
	int offset = index - virtualDataIndex;
	MoveToOffset(offset);
}

int BrainfuckTool::NewTempVariable()
{
	Right();
	return virtualDataIndex;
}

// Copies value from current memory position to offset from current memory positon, assuming that the destination contains 0
// (CURRENTLY UNTESTED)
void BrainfuckTool::CopyToOffset(int offset)
{
	int origin = virtualDataIndex;

	// First: Move origin to our counter temporary variable
	int counter = NewTempVariable();
	ChangeIndexAbsolute(origin);
	MoveToIndex(counter);

	ChangeIndexAbsolute(counter);

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

// I suspect there's a bug here, we hang on w currently
void BrainfuckTool::SubtractFromOffset(int offset)
{
	int origin = virtualDataIndex;

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
	int offset = index - virtualDataIndex;
	CopyToOffset(offset);
}

void BrainfuckTool::AddToIndex(int index)
{
	CopyToIndex(index);
}

void BrainfuckTool::SubtractFromIndex(int index)
{
	int offset = index - virtualDataIndex;
	SubtractFromOffset(offset);
}

void BrainfuckTool::SetZero()
{
	Branch();
		Minus();
	Loop();
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

void BrainfuckTool::NotInMyBackyard(int tempVariableIndex)
{
	int origin = virtualDataIndex;

	// No matter what value in the current data slot, put a 1 next door
	ChangeIndexAbsolute(tempVariableIndex);
	Plus();

	// Go back to check our original number
	ChangeIndexAbsolute(origin);

	// This checks our original number
	Branch();
		// We're not zero if we're here
		Branch();
			Minus();
		Loop();
		// Now we're 0
		ChangeIndexAbsolute(tempVariableIndex);
		Minus(); // 0 next door, so we skip the 0 logic ahead
		ChangeIndexAbsolute(origin);  // back to origin
	Loop();

	// Go right to check our neighbor
	ChangeIndexAbsolute(tempVariableIndex);

	// This checks our neighbor, which would be a 1 if our original number was a 0
	Branch();
		// Our original number was 0 if we're here
		ChangeIndexAbsolute(origin);
		Plus();		// Make our 0 into a 1
		ChangeIndexAbsolute(tempVariableIndex);
		Minus();	// Reset our neighbor
	Loop();
	ChangeIndexAbsolute(origin);
}

// I love this function, but remember, none of this will work till we add the temp variable spaces inside our level array, it depends on them
// Changing our level array to have these bubbles in it will require adjusting a few things including how we draw our levels (which is why I didn't do it before)
// I think this is a REAL legit use of the odd-temp-variable system though and I think these changes would be improvements overall
void BrainfuckTool::ChangeIndexRelativeToValueAtIndex(int index)
{
	int origin = virtualDataIndex;
	int tempIndex = NewTempVariable();

	ChangeIndexAbsolute(index);
	CopyToIndex(tempIndex);
	ChangeIndexAbsolute(tempIndex);
	Branch(false);
		// Problem: At the end of this loop we need to decrese a variable from a known position, but we can't get back to our new position after that
		// Idea: We tab over into the temp variables between here and the destination, adding 1 to them, and then moving right through all of them at the end
		// To explain more: if each loop we go out by 2 till we reach a 0 in a temp variable, then we add 1 to that temp variable and loop again, till we get where we want
		// Bonus problem: I can't "ChangeIndexAbsolute" from unknown/non-absolute coordinates, which is why I scan BACK to get to the origin instead of just "absoluting" there
		ChangeIndexAbsolute(origin);
		NewTempVariable();					// Sets us on the "temp track"; we don't care about the return value
		ChangeIndexToNextTempZero();		// Brings us to the next 0 on the "temp track"
		Plus();								// Add one on the temp track at this position, so it's not a zero next time (We'll have to clean this up later too...)
		ChangeIndexToPreviousTempZero();	// Fly back to our last untarnished 0 which should be 1 left of origin
		Right();							// <- Should be back at origin now?  Hopefully?
		ChangeIndexAbsolute(tempIndex);		// This now works because it's jumping from origin every time
		Minus();							// Finally we get to reduce our counter by 1!
	Loop();

	Plus();		// This is because I blew away tempIndex even though it's part of my breadcrumb trail

	ChangeIndexAbsolute(origin);

	// At this point we've got a pathway of ones we can follow to find where our @ symbol is (give or take 1? programmer's curse)
	// Which is where we want to end on, so we should pick up our 1 debris as we go
	Branch();
		NewTempVariable();	// Temp track
		Minus();			// Changes 1 to 0 in most cases, till we get to the end where it changes 0 to 255 or something, who cares, we'll kill it in the next loop
		Branch();
			Minus();		// Clear the final 1 so we are clean and can drop out of this creative mess
		Loop();
	Loop();

	// This means we got to a 0, which means we're very close to our @ symbol, I think a left from here will get where we want to be
	// (But it could be right, we'll see in testing)
	Left();
}

void BrainfuckTool::PlayerLogic(int wIndex, int aIndex, int sIndex, int dIndex, int wIndexTemp, int aIndexTemp, int sIndexTemp, int dIndexTemp, int playerPositionIndex, int playerPositionIndexTemp, int levelIndex, int widthIndex)
{
	int origin = virtualDataIndex;

	// Copy to our temp variables
	ChangeIndexAbsolute(wIndex);
	CopyToIndex(wIndexTemp);
	ChangeIndexAbsolute(aIndex);
	CopyToIndex(aIndexTemp);
	ChangeIndexAbsolute(sIndex);
	CopyToIndex(sIndexTemp);
	ChangeIndexAbsolute(dIndex);
	CopyToIndex(dIndexTemp);
	ChangeIndexAbsolute(playerPositionIndex);
	CopyToIndex(playerPositionIndexTemp);

	// Current index is the key pressed
	ChangeIndexAbsolute(origin);

	Branch();
		// Subtract from all till origin is 0
		ChangeIndexAbsolute(wIndexTemp);
		Minus();
		ChangeIndexAbsolute(aIndexTemp);
		Minus();
		ChangeIndexAbsolute(sIndexTemp);
		Minus();
		ChangeIndexAbsolute(dIndexTemp);
		Minus();
		ChangeIndexAbsolute(origin);
		Minus();
	Loop();	// Origin is now 0, and maybe one of w/a/s/d indexes are 0?

	ChangeIndexAbsolute(wIndexTemp);
	Not();
	Branch();
		// w was entered
		ChangeIndexAbsolute(widthIndex);
		SubtractFromIndex(playerPositionIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(wIndexTemp);
		Minus();
	Loop();

	ChangeIndexAbsolute(aIndexTemp);
	Not();
	Branch();
		// a was entered
		ChangeIndexAbsolute(playerPositionIndex);
		Minus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(aIndexTemp);
		Minus();
	Loop();

	ChangeIndexAbsolute(sIndexTemp);
	Not();
	Branch();
		// s was entered
		ChangeIndexAbsolute(widthIndex);
		AddToIndex(playerPositionIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(sIndexTemp);
		Minus();
	Loop();

	ChangeIndexAbsolute(dIndexTemp);
	Not();
	Branch();
		// d was entered
		ChangeIndexAbsolute(playerPositionIndex);
		Plus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexAbsolute(dIndexTemp);
		Minus();
	Loop();

	ChangeIndexAbsolute(wIndexTemp);
	SetZero();
	ChangeIndexAbsolute(aIndexTemp);
	SetZero();
	ChangeIndexAbsolute(sIndexTemp);
	SetZero();
	ChangeIndexAbsolute(dIndexTemp);
	SetZero();
	ChangeIndexAbsolute(playerPositionIndexTemp);
	SetZero();

	// Move the @ from the old position to the new position
	ChangeIndexAbsolute(levelIndex);
	ChangeIndexRelativeToValueAtIndex(playerPositionIndex); // This isn't what I want here.  I need to change based on the value AT playerPositionIndex, which needs a new function I think
	//MoveToIndex()

	// I'm now so close, I just need to now do the logic where the temp position stores the old address
	// One more heads up: MoveToIndex probably still expects 0 in the destination so you may have to make a better version of that function
	// Or you could always set up the game map array with nulls?  Wait that won't work because they don't print.  Nevermind.

	ChangeIndexAbsolute(origin);
}