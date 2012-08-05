#include <vector>
#include "Level.h"
#include "Tile.h"

Level::Level(int w, int h) : w(w), h(h)
{
	SetDimensions(w, h);
}

Level::~Level()
{
	for(int i = 0; i < map.size(); i++)
		for(int j = 0; j < map[i].size(); j++)
			delete map[i][j];
}

void Level::SetDimensions(int w, int h)
{
	map.resize(w);

	for(int i = 0; i < w; i++)
		map.at(i).resize(h, 0);
}

void Level::AddTile(int x, int y, Tile* tile)
{
	map[x][y] = tile;
}

Tile* Level::GetTile(int x, int y)
{
	return map[x][y];
}

void Level::LoadLevel()
{
	//tbd
}

