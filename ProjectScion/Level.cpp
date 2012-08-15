#include <vector>
#include "Level.h"
#include "Tile.h"
#include "StateManager.h"
#include "ScionEngine.h"

#include <iostream>
#include <math.h>

using namespace std;

Level::Level(int w, int h) : w(w), h(h), camera(800, 600, 0.2f)
{
	SetDimensions(w, h);

	//camera = unique_ptr<Camera>(new Camera(800, 600, 0.2f));
}

Level::~Level()
{
}

void Level::SetDimensions(int w, int h)
{
	map.resize(h);

	for(int i = 0; i < w; i++)
		map.at(i).resize(w);
}

void Level::SetTile(int x, int y, Tile&& tile)
{
	map[y][x] = tile;
}

Tile& Level::GetTile(int x, int y)
{
	return map[y][x];
}

void Level::LoadLevel()
{
	//tbd
}

Level Level::CreateLevel(int levelWidth, int levelHeight)
{
	Level& level = Level(levelWidth, levelHeight);

	//the number of rooms is proportional to the height and width
	//rough estimate is, for a 100x100 level, there should be [5,10] rooms
	
	int q1 = int(sqrt(static_cast<float>((levelWidth/10) * (levelHeight/10))));
	//ex. q1 = 10, q1/2 = 5, rand()%q1/2 = 0-4
	auto numRooms =  q1;//(int)(q1 - rand()%int(q1/2 +1));

	//fun time - create numRooms amount of 'rooms'

	vector<Room> rooms(numRooms);
	
	const int MIN_ROOM_W = 3;
	const int MAX_ROOM_W = (int)(sqrt((double)levelWidth));
	const int MIN_ROOM_H = 3;
	const int MAX_ROOM_H = (int)(sqrt((double)levelHeight));

	const int MIN_ROOM_X = 0;
	const int MAX_ROOM_X = levelWidth - 1 - MIN_ROOM_W*2;
	const int MIN_ROOM_Y = 0;
	const int MAX_ROOM_Y = levelHeight - 1 - MIN_ROOM_H*2;

	for(auto tRoom = rooms.begin(); tRoom != rooms.end(); tRoom++)
	{
		tRoom->x = MIN_ROOM_X + rand() % (MAX_ROOM_X - MIN_ROOM_X);
		tRoom->y = MIN_ROOM_Y + rand() % (MAX_ROOM_Y - MIN_ROOM_Y);
		
		auto tempW = MIN_ROOM_W + rand() % (MAX_ROOM_W - MIN_ROOM_W);
		//77 + 23 => 100 >= 80
		//100 - 81 = 19; 100 - 80 + 1 = 81
		if(tRoom->x + tempW >= levelWidth)
			tRoom->w = tempW - (tRoom->x + tempW - levelWidth) - 1;
		else
			tRoom->w = tempW;

		auto tempH = MIN_ROOM_H + rand() % (MAX_ROOM_H - MIN_ROOM_H);
		if(tRoom->y + tempH >= levelWidth)
			tRoom->h = tempH - (tRoom->y + tempH - levelHeight) - 1;
		else
			tRoom->h = tempH;
		
		auto room = *tRoom;

		//carve these tRooms out into the level
		//for(int x = tRoom->x; x < tRoom->x + tRoom->w; x++)
		//	for(int y = tRoom->y; y < tRoom->y + tRoom->h; y++)
		//		level.SetTile(x, y, Tile(ScionEngine::GetTexture("tiles.png")));
	}
	for(int x = 0; x < levelWidth; x++)
		for(int y = 0; y < levelHeight; y++)
			if(x % 5 == 0 || y % 5 == 0)
				level.SetTile(x, y, Tile(ScionEngine::GetTexture("tiles2.png")));
			else
				level.SetTile(x, y, Tile(ScionEngine::GetTexture("tiles.png")));

	return level;
}

void Level::Draw(sf::RenderWindow* window)
{
	auto bounds = camera.GetTileBounds();
	auto camOffsetX = camera.GetTileOffset().x;
	auto camOffsetY = camera.GetTileOffset().y;

	for(int y = 0, tileY = bounds.top; y < bounds.height && tileY < h; y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = bounds.left; x < bounds.width && tileX < w; x++, tileX++)
		{
		if(tileX < 0) continue;

			GetTile(tileX, tileY).Draw(tileX*Tile::SIZE, tileY*Tile::SIZE, window);
		}
	}
}