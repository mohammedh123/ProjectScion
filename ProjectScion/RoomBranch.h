#ifndef _ROOM_BRANCH_H
#define _ROOM_BRANCH_H

#include "Branch.h"
#include "LevelGenerator.h"

#include <vector>

class RoomBranch : public Branch
{
public:
	RoomBranch(std::vector<Tile*>& tiles) : Branch(tiles)
	{}

	TYPE Type() { return Branch::ROOM; }
};

#endif