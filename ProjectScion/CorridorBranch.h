#ifndef _CORR_BRANCH_H
#define _CORR_BRANCH_H

#include "Branch.h"
#include "LevelGenerator.h"

#include <vector>

class CorridorBranch : public Branch
{
public:
	CorridorBranch(std::vector<Tile*>& tiles) : Branch(tiles)
	{}

	TYPE Type() { return Branch::CORRIDOR; }

	inline bool IsFull() const
	{
		for(int i = 0; i < tiles.size(); i++)
			if(tiles[i]->type != Tile::CORRIDOR)
				return false;

		return true;
	}
};

#endif