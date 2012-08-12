#include <vector>
#include "Level.h"
#include "Tile.h"

Level::Level(int w, int h) : w(w), h(h)
{
	SetDimensions(w, h);
}

Level::~Level()
{
}

void Level::SetDimensions(int w, int h)
{
	map.resize(w);

	for(int i = 0; i < w; i++)
		map.at(i).resize(h);
}

void Level::AddTile(int x, int y, Tile&& tile)
{
	map[x][y] = tile;
}

Tile& Level::GetTile(int x, int y)
{
	return map[x][y];
}

void Level::LoadLevel()
{
	//tbd
}

Level Level::CreateLevel(int levelWidth, int levelHeight)
{
	return Level(levelWidth, levelHeight);
}

void Level::Draw(sf::RenderWindow* window)
{
	auto bounds = camera->GetTileBounds();
	auto camOffsetX = camera->GetTileOffset().x;
	auto camOffsetY = camera->GetTileOffset().y;

	for(int y = 0, tileY = 0; y < bounds.height && tileY < currentLevel->GetHeight(); y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = 0; x < bounds.width && tileX < currentLevel->GetWidth(); x++, tileX++)
		{
		if(tileX < 0) continue;

			currentLevel->GetTile(tileX, tileY).Draw(x*Tile::SIZE, y*Tile::SIZE, window);
		}
	}
}