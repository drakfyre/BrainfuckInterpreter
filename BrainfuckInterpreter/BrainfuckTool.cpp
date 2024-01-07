#include "BrainfuckTool.h"
#include <iostream>

// This currently assumes that the value we are setting is 0
void BrainfuckTool::SetValue(int value)
{
	AddValue(value);
}

void BrainfuckTool::AddValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		BrainfuckTool::Plus();
	}
}


void BrainfuckTool::SubtractValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		BrainfuckTool::Minus();
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

// This isn't working because Not is destructive and we're already using the temp variables
// So gonna bring back NotInMyBackyard (and make it actually work?)
// Or just gonna gut Not out of this
void BrainfuckTool::ChangeIndexToPreviousTempOne()
{
	Minus();
	Branch();	// Ends if we're currently at a 1
		Plus();	// Reverse what we did before our loop
		Left();	// Decrease current index
		Left();	// Decrease current index
		Minus();
	Loop();		// If current index is 1, ends

	// Do we want to clear our 1 here?
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

void BrainfuckTool::NewTempVariable()
{
	Right();
}

// Copies value from current memory position to offset from current memory positon, assuming that the destination contains 0
// (Lightly tested)
void BrainfuckTool::CopyToOffset(int offset)
{
	int origininalOffset = 0;
	int counter = 1;
	
	NewTempVariable();
	ChangeIndexRelative(origininalOffset - counter);
	MoveToOffset(counter - origininalOffset);

	ChangeIndexRelative(counter - origininalOffset);

	Branch();						// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
		ChangeIndexRelative(origininalOffset - counter);	// We change index to our origin (which now contains 0)
		Plus();									// We add 1 to our origin
		ChangeIndexRelative(offset);			// We change index to our destination
		Plus();									// We add 1 our destination
		ChangeIndexRelative(-offset);			// We go back to origin
		ChangeIndexRelative(counter - origininalOffset);	// We change index to our counter
		Minus();								// Subtract 1 from our counter
	Loop();							// Loop till counter is 0
	ChangeIndexRelative(origininalOffset - counter);
}

// I suspect there's a bug here, we hang on w currently
void BrainfuckTool::SubtractFromOffset(int offset)
{
	int originalOffset = 0;
	int counter = 1;

	NewTempVariable();
	ChangeIndexRelative(originalOffset - counter);
	MoveToOffset(counter);

	ChangeIndexRelative(counter - originalOffset);

	Branch();						// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
		ChangeIndexRelative(originalOffset - counter);	// We change index to our origin (which now contains 0)
		Plus();							// We add 1 to our origin
		ChangeIndexRelative(offset);	// We change index to our destination
		Minus();						// We subtract 1 from our destination
		ChangeIndexRelative(-offset);			// We go back to origin
		ChangeIndexRelative(counter - originalOffset);	// We change index to our counter
		Minus();						// Subtract 1 from our counter
		Loop();							// Loop till counter is 0
	ChangeIndexRelative(originalOffset - counter);
}

void BrainfuckTool::AddToOffset(int offset)
{
	CopyToOffset(offset);
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

void BrainfuckTool::ChangeIndexRelativeToValueAtOffset(int offset)
{
	int originalOffset = 0; // It's always going to be 0 but better to make it into a variable so it's named
	int tempIndex = 1;
	NewTempVariable();

	ChangeIndexRelative(offset - tempIndex);
	CopyToOffset(tempIndex - offset);
	ChangeIndexRelative(tempIndex - offset);
	Branch(true);
		// Problem: At the end of this loop we need to decrese a variable from a known position, but we can't get back to our new position after that
		// Idea: We tab over into the temp variables between here and the destination, adding 1 to them, and then moving right through all of them at the end
		// To explain more: if each loop we go out by 2 till we reach a 0 in a temp variable, then we add 1 to that temp variable and loop again, till we get where we want
		// Bonus problem: I can't "ChangeIndexAbsolute" from unknown/non-absolute coordinates, which is why I scan BACK to get to the origin instead of just "absoluting" there
		ChangeIndexToNextTempZero();		// Brings us to the next 0 on the "temp track"
		Plus();								// Add one on the temp track at this position, so it's not a zero next time (We'll have to clean this up later too...)
		ChangeIndexToPreviousTempZero();	// Fly back to our last untarnished 0 which should be 1 left of origin
		Right();							// <- Should be back at origin now?  Hopefully?
		Right();							// Now at tempIndex
		//ChangeIndexAbsolute(tempIndex);		// This now works because it's jumping from origin every time
		Minus();							// Finally we get to reduce our counter by 1!
	Loop();

	Plus();		// This is because I blew away tempIndex even though it's part of my breadcrumb trail

	// We're already on the Temp track here so I removed these commands, hopefully everything will still work (gonna break so much stuff...)
	////ChangeIndexAbsolute(origin);
	//Left();		// From tempIndex to origin
	//NewTempVariable();	// Temp track

	// At this point we've got a pathway of ones we can follow to find where our @ symbol is (give or take 1? programmer's curse)
	// Which is where we want to end on, so we should pick up our 1 debris as we go
	// Though we keep our first 1 around so that we can scan back to it!
	Right();
	Right();
	Branch();
		Minus();			// Changes 1 to 0 in most cases, till we get to the end where it changes 0 to 255 or something, who cares, we'll kill it in the next loop
		Branch();
			Minus();		// Clear the final 255 so we are clean and can drop out of this creative mess
		Loop();
		Right();
		Right();
	Loop();

	// This means we got to a 0, which means we're very close to our @ symbol, I think a left from here will get where we want to be
	// (But it could be right, we'll see in testing)
	Left();
	Left();
	Left();
}

void BrainfuckTool::ChangeIndexLeftRelativeToValueAtOffset(int offset)
{
	int originalOffset = 0; // It's always going to be 0 but better to make it into a variable so it's named
	int tempIndex = 1;
	NewTempVariable();


	ChangeIndexRelative(offset - tempIndex);
	CopyToOffset(tempIndex - offset);
	ChangeIndexRelative(tempIndex - offset);
	Branch();
		// Problem: At the end of this loop we need to decrese a variable from a known position, but we can't get back to our new position after that
		// Idea: We tab over into the temp variables between here and the destination, adding 1 to them, and then moving right through all of them at the end
		// To explain more: if each loop we go out by 2 till we reach a 0 in a temp variable, then we add 1 to that temp variable and loop again, till we get where we want
		// Bonus problem: I can't "ChangeIndexAbsolute" from unknown/non-absolute coordinates, which is why I scan BACK to get to the origin instead of just "absoluting" there
		ChangeIndexToPreviousTempZero();		// Brings us to the next 0 on the "temp track"
		Plus();								// Add one on the temp track at this position, so it's not a zero next time (We'll have to clean this up later too...)
		ChangeIndexToNextTempZero();		// Fly back to our last untarnished 0 which should be 1 right of origin
		Left();
		Left();								// Now at tempIndex
		Minus();							// Finally we get to reduce our counter by 1!
	Loop();

	Plus();		// This is because I blew away tempIndex even though it's part of my breadcrumb trail

	// We're already on the Temp track here so I removed these commands, hopefully everything will still work (gonna break so much stuff...)
	////ChangeIndexAbsolute(origin);
	//Left();		// From tempIndex to origin
	//NewTempVariable();	// Temp track

	// At this point we've got a pathway of ones we can follow to find where our @ symbol is (give or take 1? programmer's curse)
	// Which is where we want to end on, so we should pick up our 1 debris as we go
	Branch();
		Minus();			// Changes 1 to 0 in most cases, till we get to the end where it changes 0 to 255 or something, who cares, we'll kill it in the next loop
		Branch();
			Minus();		// Clear the final 255 so we are clean and can drop out of this creative mess
		Loop();
		Left();
		Left();
	Loop();


	Right();
}

void BrainfuckTool::PlayerLogic(int inputCharacterIndex, int wIndex, int aIndex, int sIndex, int dIndex, int wIndexTemp, int aIndexTemp, int sIndexTemp, int dIndexTemp, int playerPositionIndex, int playerPositionIndexTemp, int levelIndex, int widthIndex)
{
	int origin = inputCharacterIndex;

	// Copy to our temp variables
	ChangeIndexRelative(wIndex - origin);
	CopyToOffset(wIndexTemp - wIndex);
	ChangeIndexRelative(aIndex - wIndex);
	CopyToOffset(aIndexTemp - aIndex);
	ChangeIndexRelative(sIndex - aIndex);
	CopyToOffset(sIndexTemp - sIndex);
	ChangeIndexRelative(dIndex - sIndex);
	CopyToOffset(dIndexTemp - dIndex);
	ChangeIndexRelative(playerPositionIndex - dIndex);
	CopyToOffset(playerPositionIndexTemp - playerPositionIndex);

	// Current index is the key pressed
	ChangeIndexRelative(origin - playerPositionIndex);

	Branch();
		// Subtract from all till origin is 0
		ChangeIndexRelative(wIndexTemp - origin);
		Minus();
		ChangeIndexRelative(aIndexTemp - wIndexTemp);
		Minus();
		ChangeIndexRelative(sIndexTemp - aIndexTemp);
		Minus();
		ChangeIndexRelative(dIndexTemp - sIndexTemp);
		Minus();
		ChangeIndexRelative(origin - dIndexTemp);
		Minus();
	Loop();	// Origin is now 0, and maybe one of w/a/s/d indexes are 0?

	ChangeIndexRelative(wIndexTemp - origin);
	Not();
	Branch();
		// w was entered
		ChangeIndexRelative(widthIndex - wIndexTemp);
		SubtractFromOffset(playerPositionIndex - widthIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexRelative(wIndexTemp - widthIndex);
		Minus();
	Loop();

	ChangeIndexRelative(aIndexTemp - wIndexTemp);
	Not();
	Branch();
		// a was entered
		ChangeIndexRelative(playerPositionIndex - aIndexTemp);
		Minus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexRelative(aIndexTemp - playerPositionIndex);
		Minus();
	Loop();

	ChangeIndexRelative(sIndexTemp - aIndexTemp);
	Not();
	Branch();
		// s was entered
		ChangeIndexRelative(widthIndex - sIndexTemp);
		AddToOffset(playerPositionIndex - widthIndex);

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexRelative(sIndexTemp - widthIndex);
		Minus();
	Loop();

	ChangeIndexRelative(dIndexTemp - sIndexTemp);
	Not();
	Branch();
		// d was entered
		ChangeIndexRelative(playerPositionIndex - dIndexTemp);
		Plus();

		// This gets us back to the same position as if we didn't branch so that our "absolute" functions still work
		ChangeIndexRelative(dIndexTemp - playerPositionIndex);
		Minus();
	Loop();

	ChangeIndexRelative(wIndexTemp - dIndexTemp);
	SetZero();
	ChangeIndexRelative(aIndexTemp - wIndexTemp);
	SetZero();
	ChangeIndexRelative(sIndexTemp - aIndexTemp);
	SetZero();
	ChangeIndexRelative(dIndexTemp - sIndexTemp);
	SetZero();

	// Move the @ from the old position to the new position
	ChangeIndexRelative(levelIndex - dIndexTemp);
	ChangeIndexRelativeToValueAtOffset(playerPositionIndexTemp - levelIndex);
	SubtractValue(32); // Did you know that @ - 32 = Space?
	Left();

	// We don't know where we are coming from so we can't know where the indexes are in relationship
	// What we do here is keep a 1 from the breadcrumb trail to scan back to instead
	ChangeIndexToPreviousTempOne();
	Left();

	ChangeIndexRelativeToValueAtOffset(playerPositionIndex - levelIndex);
	AddValue(32); // Did you know that Space + 32 = @?  You should!
	Left();

	// We don't know where we are coming from so we can't know where the indexes are in relationship
	// What we do here is keep a 1 from the breadcrumb trail to scan back to instead
	ChangeIndexToPreviousTempOne();
	Left();

	ChangeIndexRelative(playerPositionIndexTemp - levelIndex);
	SetZero();

	ChangeIndexRelative(origin - playerPositionIndexTemp);
}