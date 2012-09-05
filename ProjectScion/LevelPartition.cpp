#include "LevelPartition.h"

void LEVEL_PARTITION::split(bool hSplit, int splitPos, LEVEL_PARTITION& a, LEVEL_PARTITION& b)
{
	LEVEL_PARTITION copyOfThis(*this);

	if(hSplit)
	{
		int oldH = copyOfThis.h;
		copyOfThis.h = splitPos - copyOfThis.y;

		a = copyOfThis;
		b = LEVEL_PARTITION(copyOfThis.x, splitPos, copyOfThis.w, oldH-copyOfThis.h);
	}
	else
	{
		int oldW = copyOfThis.w;
		copyOfThis.w = splitPos - copyOfThis.x;
			
		a = copyOfThis;
		b = LEVEL_PARTITION(splitPos, copyOfThis.y, oldW-copyOfThis.w, copyOfThis.h);
	}
}

bool LEVEL_PARTITION::testSplit(bool hSplit, int splitPos, int minRoomSize)
{
	LEVEL_PARTITION copyOfThis(*this);

	if(!hSplit &&
		(copyOfThis.x + copyOfThis.w <= splitPos ||
		splitPos <= copyOfThis.x ||
		splitPos - x < minRoomSize
		))
		return false;
	if(hSplit &&
		(copyOfThis.y + copyOfThis.h <= splitPos ||
		splitPos <= copyOfThis.y ||
		splitPos - y < minRoomSize))
		return false;
		
	LEVEL_PARTITION tempA, tempB;
	copyOfThis.split(hSplit, splitPos, tempA, tempB);

	return tempA.isValid(minRoomSize) && tempB.isValid(minRoomSize) &&
		tempA.area() >= minRoomSize*minRoomSize && 
		tempB.area() >= minRoomSize*minRoomSize;
}

bool LEVEL_PARTITION::testSplitAll(bool hSplit, int minRoomSize)
{
	if(!hSplit)
	{
		for(int i = x + 1; i < x + w - 1; i++)
			if(testSplit(hSplit, i, minRoomSize))
				return true;
	}
	else
	{
		for(int i = y + 1; i < y + h - 1; i++)
			if(testSplit(hSplit, i, minRoomSize))
				return true;
	}

	return false;
}
																												
bool operator==(const LEVEL_PARTITION& lhs, const LEVEL_PARTITION& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
}