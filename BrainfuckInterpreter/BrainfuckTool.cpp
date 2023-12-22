#include "BrainfuckTool.h"
#include <iostream>

// This currently assumes that the value we are setting is 0
void BrainfuckTool::SetValue(int value)
{
	for (int i = 0; i < value; i++)
	{
		//std::cout << '+';
		BrainfuckTool::Add();
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

void BrainfuckTool::Right(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		//std::cout << '>';
		bfString += '>';
	}
}

void BrainfuckTool::Left(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		//std::cout << '<';
		bfString += '<';
	}
}

void BrainfuckTool::Add()
{
	bfString += '+';
}

void BrainfuckTool::Minus()
{
	bfString += '-';
}

void BrainfuckTool::Out(int length)
{
	for (int i = 0; i < length; i++)
	{
		bfString += '.';
		Right();
	}
}

void BrainfuckTool::Branch()
{
	bfString += '[';
}

void BrainfuckTool::Loop()
{
	bfString += ']';
}

// Copies value from current memory position to offset from current memory positon, assuming that the destination contains 0
// (CURRENTLY INCOMPLETE)
void BrainfuckTool::CopyTo(int offset)
{
	// I've never brainfucked before so bear with me
	// We kinda can't make this non-destructive in some way, at least in interim
	// We may need to have an offset to a scratch pad area, but let's try to avoid that
	// We will currently assume that the destination contains 0, but we'll circle back around on that later
	// We are done copying if the current memory position value is 0 (at which point we'll restore it somehow)
	Branch();		// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
	Right(offset);	// We move to our destination
	Add();			// We add 1 to our destination
	Left(offset);	// We move back to our source
	Minus();		// We subtract 1 from our source
	Loop();			// If our source is 0, stop looping

	// At this point I *think* we've now put a 0 in our original location, and our original value is now at offset
	// Yeah... there's no way to avoid a scratch pad for a copy, I don't think
	// As this is basically a functional "MoveTo" I've added that function with the code above
	// Also looking more and more that it would be helpful to have some kind of assembler/memory pointer tracking system...
	// If not... we should probably put an iterator slot in a prime position (0?)
	// But doing that moves the level somewhere else, again indicating that an assembler/pointer tracking system would be useful...
}

// Moves value from current memory position to offset from current memory position, assuming that the destination contains 0
// Not sure how useful this actually is, just happened to write it by accident when trying to make a more useful function
// (CURRENTLY UNTESTED)
void BrainfuckTool::MoveTo(int offset)
{
	Branch();		// If current value is 0 we end up at associated loop; we'd be done in that case anyway so that's perfect
	Right(offset);	// We move to our destination
	Add();			// We add 1 to our destination
	Left(offset);	// We move back to our source
	Minus();		// We subtract 1 from our source
	Loop();			// If our source is 0, stop looping
}