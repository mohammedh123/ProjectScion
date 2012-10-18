#ifndef _CORR_BRANCH_H
#define _CORR_BRANCH_H

#include "Branch.h"
#include "LevelGenerator.h"

#include <vector>

class CorridorBranch : public Branch
{
public:
    CorridorBranch(std::vector<Tile*>& tiles, Branch* parent = nullptr) : Branch(tiles, parent)
    {}

    TYPE Type() { return Branch::CORRIDOR; }

    inline bool IsFull() const
    {
        for(size_t i = 0; i < tiles.size(); i++)
            if(tiles[i]->type != Tile::CORRIDOR)
                return false;

        return true;
    }
};

#endif