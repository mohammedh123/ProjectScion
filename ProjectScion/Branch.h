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
	Branch* parent;

	Branch(std::vector<Tile*>& tiles, Branch* parent=nullptr) : tiles(tiles), parent(parent)
	{}

	virtual TYPE Type() = 0;
};

#endif