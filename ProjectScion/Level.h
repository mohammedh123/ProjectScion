#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include "Tile.h"

class Level
{
private:
	std::vector<std::vector<Tile*>> map;

	int w;
	int h;

	void SetDimensions(int w, int h);	
public:
	Level(int widthInTiles, int heightInTiles);
	~Level();

	void AddTile(int x, int y, Tile* tile);
	Tile* GetTile(int x, int y);

	void LoadLevel();

	inline int GetWidth() const {return w;}
	inline int GetHeight() const {return h;}
};

#endif

