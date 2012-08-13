#include <vector>
#include "Level.h"
#include "Tile.h"
#include "StateManager.h"
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
	auto level = Level(levelWidth, levelHeight);

	//the number of rooms is proportional to the height and width
	//rough estimate is, for a 100x100 level, there should be [5,10] rooms
	
	auto q1 = sqrt(static_cast<float>((levelWidth/10) * (levelHeight/10)));
	//ex. q1 = 10, q1/2 = 5, rand()%q1/2 = 0-4
	auto numRooms =  q1 - rand()%int(q1/2 +1);

	//fun time - create numRooms amount of 'rooms'
	typedef struct
	{
		int x, y, w, h;
	} Room;

	vector<Room> rooms(numRooms);
	
	const int MIN_ROOM_W = 1;
	const int MAX_ROOM_W = levelWidth/2;
	const int MIN_ROOM_H = 1;
	const int MAX_ROOM_H = levelHeight/2;

	const int MIN_ROOM_X = 0;
	const int MAX_ROOM_X = levelWidth - MIN_ROOM_W*2;
	const int MIN_ROOM_Y = 0;
	const int MAX_ROOM_Y = levelHeight - MIN_ROOM_H*2;

	for_each(begin(rooms), end(rooms), [&](Room& room)
	{
		room.x = MIN_ROOM_X + rand() % (MAX_ROOM_X - MIN_ROOM_X);
		room.y = MIN_ROOM_Y + rand() % (MAX_ROOM_Y - MIN_ROOM_Y);
		room.w = MIN_ROOM_W + rand() % (MAX_ROOM_W - MIN_ROOM_W);
		room.h = MIN_ROOM_H + rand() % (MAX_ROOM_H - MIN_ROOM_H);

		//carve these rooms out into the level
		for(int x = room.x; x < room.x + room.w; x++)
			for(int y = room.y; y < room.y + room.h; y++)
				level.GetTile(x, y);
	});

	return level;
}

void Level::Draw(sf::RenderWindow* window)
{
	auto bounds = camera.GetTileBounds();
	auto camOffsetX = camera.GetTileOffset().x;
	auto camOffsetY = camera.GetTileOffset().y;

	for(int y = 0, tileY = 0; y < bounds.height && tileY < h; y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = 0; x < bounds.width && tileX < w; x++, tileX++)
		{
		if(tileX < 0) continue;

			GetTile(tileX, tileY).Draw(x*Tile::SIZE, y*Tile::SIZE, window);
		}
	}
}