#ifndef _BRANCH_H
#define _BRANCH_H

#include <vector>

#include "Tile.h"

struct Branch
{
public:
	enum TYPE
	{
		CORRIDOR,
		ROOM
	};

	std::vector<Tile*> tiles;
	std::vector<Branch*> children;

	Branch(std::vector<Tile*>& tiles) : tiles(tiles)
	{}

	virtual TYPE Type() = 0;
};

#endif