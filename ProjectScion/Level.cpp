#include <vector>
#include <math.h>
#include "Level.h"
#include "Tile.h"
#include "StateManager.h"
#include "ScionEngine.h"

#include <noise/noise.h>
#include "noiseutils.h"
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <functional>

using namespace std;
using namespace noise;

Level::Level(int w, int h) : w(w), h(h), camera(800, 600, 1.0f)
{
	SetDimensions(w, h);

	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++)
			SetTile(i, j, Tile::DefaultTiles().at(TILE_TYPE::BLANK));


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

void Level::SetTile(int x, int y, const Tile& tile)
{
	map[y][x] = tile;
}

Tile& Level::GetTile(int x, int y)
{
	return map.at(y).at(x);
}

void Level::LoadLevel()
{
	//tbd
}

Level::Room Level::CreateRoom(vector<Room>& rooms, Room::ROOM_TYPE type, Room::ROOM_SIZE size, int levelWidth, int levelHeight)
{
	Room room;
	
	const int MIN_ROOM_W = 3;
	const int MAX_ROOM_W = size;
	const int MIN_ROOM_H = 3;
	const int MAX_ROOM_H = size;

	room.type = type;
	
	switch(type)
	{
	case Room::ROOM_TYPE::RECTANGULAR:
		{
			int numAttemptsToCreateRoom = 0;
			bool anyCollisions = false;

			do
			{
				room.x = room.y = room.w = room.h = 0;

				anyCollisions = false;
				while(room.w % 2 == 0)
					room.w = ScionEngine::GetRandomNumber(MIN_ROOM_W, MAX_ROOM_W);

				while(room.h % 2 == 0)
					room.h = ScionEngine::GetRandomNumber(MIN_ROOM_H, MAX_ROOM_H);

				while(room.x % 2 == 0 || room.x + room.w >= levelWidth)
					room.x = ScionEngine::GetRandomNumber(1, levelWidth-1);
			
				while(room.y % 2 == 0 || room.y + room.h >= levelHeight)
					room.y = ScionEngine::GetRandomNumber(1, levelHeight-1);

				for_each(begin(rooms), end(rooms), [&](Room& room2)
				{
					if(room2.CollidesWith(room))
						anyCollisions = true;
				});

				numAttemptsToCreateRoom++;
			}
			while(anyCollisions && numAttemptsToCreateRoom < 16);

			//if(numAttemptsToCreateRoom >= 10)
			//{
			//	room.x = room.y = room.w = room.h = 0;

			//	return room;
			//}

			//auto anotherRoomChance = ScionEngine::GetRandomNumber(1, 10);

			//if(anotherRoomChance <= 1)
			//{
			//	auto centerX = int(room.x + room.w * 0.5);
			//	auto centerY = int(room.y + room.h * 0.5);

			//	Room room2 = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, size, levelWidth, levelHeight);
			//	room2.x = centerX - room2.w;
			//	room2.y = centerY - room2.h;
			//}

			break;
		}
	case Room::ROOM_TYPE::STARBURST:
		break;
	case Room::ROOM_TYPE::ROTATED:
		break;
	case Room::ROOM_TYPE::CIRCULAR:
		break;
	}

	rooms.push_back(room);

	return room;
}

Level Level::CreateLevelWithRooms(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);

	auto numRooms =  (levelSize*levelSize)/(maxRoomSize*maxRoomSize);

	//fun time - create numRooms amount of 'rooms'

	vector<Room> rooms;
	rooms.reserve(numRooms);

	for(int i = 0; i < numRooms; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		Room& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		sf::Color c(ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255), 80);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
			{
				level.SetTile(x, y, Tile::DefaultTiles().at(GROUND));
				level.GetTile(x, y).color *= c;
			}
	}

	for(int x =0; x < levelSize; x++)
	{
		level.SetTile(x, 0, Tile::DefaultTiles().at(WALL));
		level.SetTile(x, levelSize-1, Tile::DefaultTiles().at(WALL));
	}

	for(int y = 0; y < levelSize; y++)
	{
		level.SetTile(0, y, Tile::DefaultTiles().at(WALL));
		level.SetTile(levelSize-1, y, Tile::DefaultTiles().at(WALL));
	}

	//for(int x =0; x < levelSize; x++)
	//	for(int y = 0; y < levelSize; y++)
	//	{
	//		Tile& t = level.GetTile(x, y);
	//		
	//		auto n = 1;
	//		int numSolid = 0;

	//		if(t.type == GROUND)
	//		{
	//			for(int xx = x - n; xx <= x + n; xx++)
	//			{
	//				for(int yy = y - n; yy <= y + n; yy++)
	//				{
	//					if(xx == x && yy == y)
	//						continue;

	//					try
	//					{
	//						if(level.GetTile(xx, yy).type == UNUSED)
	//							level.SetTile(xx, yy, Tile::DefaultTiles().at(WALL));
	//					}
	//					catch(out_of_range e)
	//					{
	//					}
	//				}
	//			}
	//		}
	//	}

	return level;
}

Level Level::CreateLevelWithPerlinNoise(int levelWidth, int levelHeight)
{
	Level& level = Level(levelWidth, levelHeight);

	module::Perlin myModule;
	myModule.SetSeed(GetTickCount());
	//utils::NoiseMap heightMap;
	//utils::NoiseMapBuilderPlane heightMapBuilder;
	//heightMapBuilder.SetSourceModule (myModule);
	//heightMapBuilder.SetDestNoiseMap (heightMap);
	//heightMapBuilder.SetDestSize (100, 100);
	//auto width = 10.0;
	//heightMapBuilder.SetBounds (0.0, width, 0.0, width);
	//heightMapBuilder.Build ();

	//utils::RendererImage renderer;
	//utils::Image image;
	//renderer.SetSourceNoiseMap (heightMap);
	//renderer.SetDestImage (image);
	//renderer.ClearGradient ();
	//renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	//renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	//renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
	//renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
	//renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	//renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	//renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	//renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
	//renderer.Render();

	auto randomZ = rand();
	for(int x = 0; x < levelWidth; x++)
		for(int y = 0; y < levelHeight; y++)
		{
			double value = myModule.GetValue (x*0.01, y*0.01, 0);

			if(value > 0)
				level.SetTile(x, y, Tile::DefaultTiles().at(GROUND));
		}

		return level;
}

Level Level::CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight)
{
	Level& level = Level(levelWidth, levelHeight);

	module::Voronoi myModule;
	myModule.SetSeed(GetTickCount());
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (myModule);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize (100, 100);
	auto width = 6.0;
	heightMapBuilder.SetBounds (0.0, width, 0.0, width);
	heightMapBuilder.Build ();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
	renderer.Render();

	auto randomZ = rand();
	for(int x = 0; x < levelWidth; x++)
		for(int y = 0; y < levelHeight; y++)
		{
			auto col = image.GetSlabPtr(x, y);
			//double value = myModule.GetValue (x*0.005, y*0.005, randomZ);

			//if(value > 0)
			Tile t = Tile::DefaultTiles().at(GROUND);
			t.color = sf::Color(col->red, col->green, col->blue, col->alpha);
			level.SetTile(x, y, t);
		}

		return level;
}

Level Level::CreateLevelWithCA(int levelWidth, int levelHeight)
{
	Level& level = Level(levelWidth, levelHeight);
	//border the level
	//for(int i = 0; i < levelWidth; i++)
	//{
	//	level.SetTile(i, 0, Tile(ScionEngine::GetTexture("tiles.png")));
	//	level.SetTile(0, i, Tile(ScionEngine::GetTexture("tiles.png")));
	//	level.SetTile(i, levelHeight-1, Tile(ScionEngine::GetTexture("tiles.png")));
	//	level.SetTile(levelWidth-1, i, Tile(ScionEngine::GetTexture("tiles.png")));
	//}

	const float INITIAL_FILL = 0.40f;

	float currentFill = 0.0f;
	float inverse = 1.0f/(levelWidth*levelHeight);

	while(currentFill < INITIAL_FILL)
	{
		level.SetTile(ScionEngine::GetRandomNumber(0, levelWidth-1), ScionEngine::GetRandomNumber(0, levelHeight-1), Tile::DefaultTiles().at(GROUND));

		currentFill += inverse;
	}

	const int NUM_CA_GENERATIONS = 2;
	auto lam = [&level](int x, int y, int n, int& numSolid)
	{		
		for(int xx = x - n; xx <= x + n; xx++)
		{
			for(int yy = y - n; yy <= y + n; yy++)
			{
				if(xx == x && yy == y)
					continue;

				try
				{
					if(level.GetTile(xx, yy).solid)
						numSolid++;
				}
				catch(out_of_range e)
				{
				}
			}
		}
	};

	for(int u = 0; u < NUM_CA_GENERATIONS; u++)
	{
		for(int x = 0; x < levelWidth; x++)
			for(int y = 0; y < levelHeight; y++)
			{
				//get number of surrounding tiles that are solid
				int numSolid = 0;
				int numSolid2 = 0;
				
				lam(x, y, 1, numSolid);
				//lam(x, y, 3, numSolid2);

				if(numSolid >= 4)
				{
					level.SetTile(x, y, Tile::DefaultTiles().at(GROUND));
				}
				else if(numSolid < 4)
				{
					level.SetTile(x, y, Tile::DefaultTiles().at(BLANK));
				}
			}
	}

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