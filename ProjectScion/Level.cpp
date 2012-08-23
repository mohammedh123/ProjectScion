#include <vector>
#include <math.h>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
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
#include <algorithm>
#include <deque>
#include <array>

using namespace std;
using namespace noise;

namespace std
{
	template<>
	struct hash<Tile>
	{
		std::size_t operator()(const Tile& tile) const
		{
			return std::hash<int>()(tile.x) ^ std::hash<int>()(tile.y);
		}
	};
}

Level::Level(int w, int h) : w(w), h(h), camera(800, 600, 1.0f)
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

	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++)
			map[j][i] = Tile(i, j);
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
					if(room2.CollidesWith(room, 0))
						anyCollisions = true;
				});

				numAttemptsToCreateRoom++;
			}
			while(anyCollisions && numAttemptsToCreateRoom < 16);

			if(numAttemptsToCreateRoom >= 16)
			{
				room.x = room.y = room.w = room.h = 0;

				return room;
			}

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

Level Level::CreateLevelWithRooms2(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);

	int numRooms =  ((levelSize-2)*(levelSize-2))/(maxRoomSize*maxRoomSize);

	//fun time - create numRooms amount of 'rooms'

	vector<Room> rooms;
	rooms.reserve(numRooms);

	for(int i = 0; i < 0; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		Room& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
				level.SetTile(x, y, GROUND);
	}

	level.FindPath(level.GetTile(5, 5), level.GetTile(6,5));

	return level;
}

Level Level::CreateLevelWithRooms1(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);

	int numRooms =  ((levelSize-2)*(levelSize-2))/(maxRoomSize*maxRoomSize);

	//fun time - create numRooms amount of 'rooms'

	vector<Room> rooms;
	rooms.reserve(numRooms);

	for(int i = 0; i < numRooms; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		Room& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
				level.SetTile(x, y, GROUND);
	}

	for(int x =0; x < levelSize; x++)
	{
		level.SetTile(x, 0, WALL);
		level.SetTile(x, levelSize-1, WALL);
	}

	for(int y = 0; y < levelSize; y++)
	{
		level.SetTile(0, y, WALL);
		level.SetTile(levelSize-1, y, WALL);
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
	//							level.SetTile(xx, yy, WALL);
	//					}
	//					catch(out_of_range e)
	//					{
	//					}
	//				}
	//			}
	//		}
	//	}


	//generate corridors
	for(int x = 1; x < level.GetWidth(); x+=2)
		for(int y = 1; y < level.GetWidth(); y+=2)
		{
			const auto& t = level.GetTile(x, y);

			if(t != Tile::DefaultTiles().at(BLANK) && t != Tile::DefaultTiles().at(UNUSED))
				continue;

			level.OpenCorridor(x, y);
		}

		return level;
}

void Level::OpenCorridor(int x, int y, sf::Vector2i direction)
{
	static sf::Vector2i fourDirections[4] = 
	{
		sf::Vector2i(0, -1),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(-1, 0)
	};

	deque<sf::Vector2i> dirs;

	for(auto dir = begin(fourDirections); dir != end(fourDirections); dir++)
	{
		dirs.push_back(*dir);
	}

	random_shuffle(begin(dirs), end(dirs));

	if(ScionEngine::GetRandomNumber() < 95)		
		dirs.push_front(direction);

	for(auto dir = begin(dirs); dir != end(dirs); dir++)
	{
		//attempt to open a tunnel in this direction
		if(OpenTunnel(x, y, *dir))
		{
			auto nextX = x + dir->x*2;
			auto nextY = y + dir->y*2;

			OpenCorridor(nextX, nextY, *dir);
		}
	}
}

bool Level::OpenTunnel(int x, int y, sf::Vector2i direction)
{
	auto nextX = x + 2*direction.x;
	auto nextY = y + 2*direction.y;

	int midX = (x + nextX)/2;
	int midY = (y + nextY)/2;

	if(IsTunnelValid(midX, midY, nextX, nextY))
		return CreateTunnel(x, y, nextX, nextY);
	else
		return false;
}

bool Level::IsTunnelValid(int midX, int midY, int nextX, int nextY)
{
	if(nextX < 0 || nextX >= w)
		return false;
	if(nextY < 0 || nextY >= h)
		return false;

	int minX = min(midX, nextX);
	int maxX = max(midX, nextX);
	int minY = min(midY, nextY);
	int maxY = max(midY, nextY);

	for(int x = minX; x <= maxX; x++)
		for(int y = minY; y <= maxY; y++)
		{
			if(GetTile(x, y).type != TILE_TYPE::BLANK && GetTile(x,y).type != TILE_TYPE::UNUSED)
				return false;
		}

		return true;
}

bool Level::CreateTunnel(int thisX, int thisY, int nextX, int nextY)
{	
	int minX = min(thisX, nextX);
	int maxX = max(thisX, nextX);
	int minY = min(thisY, nextY);
	int maxY = max(thisY, nextY);

	for(int x = minX; x <= maxX; x++)
		for(int y = minY; y <= maxY; y++)
			if(GetTile(x, y) != Tile::DefaultTiles().at(GROUND))
				SetTile(x, y, CORRIDOR);

	//GetTile(maxX, maxY).color = sf::Color(255, 120, 0, 255);

	return true;
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
				level.SetTile(x, y, GROUND);
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
		level.SetTile(ScionEngine::GetRandomNumber(0, levelWidth-1), ScionEngine::GetRandomNumber(0, levelHeight-1), GROUND);

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
					level.SetTile(x, y, GROUND);
				}
				else if(numSolid < 4)
				{
					level.SetTile(x, y, BLANK);
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

			GetTile(tileX, tileY).Draw(window);
		}
	}
}

//path finding stuff

float Level::HeuristicForNode(const Tile& start, const Tile& end)
{
	//used euclidean distance
	return (float)std::sqrtf(Tile::SIZE*(end.x - start.x) * Tile::SIZE*(end.x - start.x) + Tile::SIZE*(end.y - start.y) * Tile::SIZE*(end.y - start.y));
}

float Level::GetDistance(const Tile& s, const Tile& e)
{
	//returns the distance between 2 nodes
	int xD = abs(e.x - s.x);
	int yD = abs(e.y - s.y);

	if (xD > yD || yD > xD) //1,0 or 0,1
		return 1.0f;
	else if (xD == yD) //1,1
	{
		if (!GetTile(s.x + (e.x - s.x), s.y).solid && !GetTile(s.x, s.y + (e.y - s.y)).solid)
			return 0.9f;
		else
			return 140.0f;
	}
	else
		return 0;
}

queue<Tile> Level::GetNeighbors(const Tile& n)
{
	//returns the neighbors of a node

	queue<Tile> neighbors;

	if (n.x > 0)
	{
		neighbors.push(GetTile(n.x - 1, n.y));

		if (n.y > 0)
			neighbors.push(GetTile(n.x - 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push(GetTile(n.x - 1, n.y + 1));
	}

	if (n.y > 0)
		neighbors.push(GetTile(n.x, n.y - 1));

	if (n.x < w - 1)
	{
		neighbors.push(GetTile(n.x + 1, n.y));

		if (n.y > 0)
			neighbors.push(GetTile(n.x + 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push(GetTile(n.x + 1, n.y + 1));
	}

	if (n.y < h - 1)
		neighbors.push(GetTile(n.x, n.y + 1));

	return neighbors;
}

queue<Tile>& Level::ConstructPath(std::unordered_map<Tile, Tile>& q, queue<Tile>& path, const Tile& t)
{
	//traverses the map and constructs a path given a node
	if (q.find(t) != q.end())
	{
		path = ConstructPath(q, path, q[t]);
		path.push(t);
		return path;
	}
	else
		return path;
}

queue<Tile> Level::FindPath(const Tile& start, const Tile& end)
{
	bool newGScoreBetter = false;
	unordered_set<Tile> closedSet;
	unordered_set<Tile> openSet;
	openSet.insert(start);

	std::unordered_map<Tile, Tile> navigated;
	std::unordered_map<Tile, float> gScore;
	std::unordered_map<Tile, float> hScore;
	std::map<float, queue<Tile>> fScore; //easy way to use a structure to auto-sort for you

	gScore[start] = 0;
	hScore[start] = HeuristicForNode(start, end);
	fScore[gScore[start] + hScore[start]].push(start);

	while (openSet.size() > 0)
	{
		//take the lowest f-score node and dequeue it
		auto& lowestScore = fScore.begin()->second;
		Tile current = lowestScore.front();
		lowestScore.pop();

		if (lowestScore.size() == 0)
			fScore.erase(fScore.begin()); //just some maintenance to make sure you keep the lowest fscore with .First()

		if (current == end) //reached the end
		{
			queue<Tile> path;
			path.push(start);
			return ConstructPath(navigated, path, current);
		}

		openSet.erase(current); //remove current from open
		closedSet.insert(current); //put into closed

		auto neighbors = GetNeighbors(current);
		for(auto neighbor1 = std::begin(neighbors._Get_container()); neighbor1 != std::end(neighbors._Get_container()); neighbor1++)
		{
			auto& neighbor = *neighbor1;
			if (closedSet.find(neighbor) != closedSet.end() || neighbor.solid)
				continue;

			//newgscore is gscore + 1*distance
			float newGScore = gScore[current] + 1*GetDistance(current, neighbor);

			if (openSet.find(neighbor) == openSet.end())
			{
				openSet.insert(neighbor);
				hScore[neighbor] = HeuristicForNode(neighbor, end);
				newGScoreBetter = true;
			}
			else if (newGScore < gScore[neighbor])
				newGScoreBetter = true;
			else
				newGScoreBetter = false;

			if (newGScoreBetter)
			{
				navigated[neighbor] = current;
				gScore[neighbor] = newGScore;

				if (fScore.find(gScore[neighbor] + hScore[neighbor]) == fScore.end())
					fScore[gScore[neighbor] + hScore[neighbor]];

				fScore[gScore[neighbor] + hScore[neighbor]].push(neighbor);
			}
		}
	}

	//failed
	return queue<Tile>();
}
