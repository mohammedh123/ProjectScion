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
	
	const int MIN_ROOM_W = floor(size/2.0f);
	const int MAX_ROOM_W = size;
	const int MIN_ROOM_H = floor(size/2.0f);
	const int MAX_ROOM_H = size;

	const int MIN_ROOM_X = 1;
	const int MAX_ROOM_X = levelWidth - 2 - MIN_ROOM_W;
	const int MIN_ROOM_Y = 1;
	const int MAX_ROOM_Y = levelHeight - 2 - MIN_ROOM_H;

	room.type = type;
	
	switch(type)
	{
	case Room::ROOM_TYPE::RECTANGULAR:
		{
			room.x = ScionEngine::GetRandomNumber(MIN_ROOM_X, MAX_ROOM_X);
			room.y = ScionEngine::GetRandomNumber(MIN_ROOM_Y, MAX_ROOM_Y);

			auto tempW = ScionEngine::GetRandomNumber(MIN_ROOM_W, MAX_ROOM_W);

			if(room.x + tempW >= levelWidth)
				room.w = tempW - (room.x + tempW - levelWidth) - 1;
			else
				room.w = tempW;

			auto tempH = ScionEngine::GetRandomNumber(MIN_ROOM_H, MAX_ROOM_H);
			if(room.y + tempH >= levelWidth)
				room.h = tempH - (room.y + tempH - levelHeight) - 1;
			else
				room.h = tempH;

			auto anotherRoomChance = ScionEngine::GetRandomNumber(1, 10);

			if(anotherRoomChance <= 1)
			{
				auto centerX = int(room.x + room.w * 0.5);
				auto centerY = int(room.y + room.h * 0.5);

				Room room2 = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, size, levelWidth, levelHeight);
				room2.x = centerX - room2.w;
				room2.y = centerY - room2.h;
			}

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

Level Level::CreateLevelWithRooms(int levelWidth, int levelHeight)
{
	Level& level = Level(levelWidth, levelHeight);

	//the number of rooms is proportional to the height and width
	//rough estimate is, for a 100x100 level, there should be [5,10] rooms

	int q1 = int(sqrt(static_cast<float>((levelWidth/3 * levelHeight/3))));
	//ex. q1 = 10, q1/2 = 5, rand()%q1/2 = 0-4
	auto numRooms =  q1;//(int)(q1 - rand()%int(q1/2 +1));

	//fun time - create numRooms amount of 'rooms'

	vector<Room> rooms;
	rooms.reserve(numRooms);

	for(int i = 0; i < numRooms; i++)
	{
		Room::ROOM_SIZE size;

		int randNum = ScionEngine::GetRandomNumber(1, 10);

		if(randNum <= 2) // 20% chance of small	
			size = Room::ROOM_SIZE::SMALL;
		else if(randNum <= 9) // 70% chance of medium
			size = Room::ROOM_SIZE::NORMAL;
		else if(randNum <= 10) // 10% chance of large
			size = Room::ROOM_SIZE::LARGE;		

		CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, size, levelWidth, levelHeight);
	}
	
	for(auto tRoom = rooms.begin(); tRoom != rooms.end(); tRoom++)
	{
		//carve these tRooms out into the level
		for(int x = tRoom->x; x < tRoom->x + tRoom->w; x++)
			for(int y = tRoom->y; y < tRoom->y + tRoom->h; y++)
				level.SetTile(x, y, Tile::DefaultTiles().at(GROUND));
	}

	for(int x =0; x < levelWidth; x++)
		for(int y = 0; y < levelHeight; y++)
		{
			Tile& t = level.GetTile(x, y);
			
			auto n = 1;
			int numSolid = 0;

			if(t.type == GROUND)
			{
				for(int xx = x - n; xx <= x + n; xx++)
				{
					for(int yy = y - n; yy <= y + n; yy++)
					{
						if(xx == x && yy == y)
							continue;

						try
						{
							if(level.GetTile(xx, yy).type == UNUSED)
								level.SetTile(xx, yy, Tile::DefaultTiles().at(WALL));
						}
						catch(out_of_range e)
						{
						}
					}
				}
			}
		}

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

	auto randomZ = std::rand();
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

	auto randomZ = std::rand();
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