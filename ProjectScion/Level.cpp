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

struct SUB_LEVEL
{
	SUB_LEVEL* left, *right, *parent;

	int x,y,w,h;

	SUB_LEVEL(int x, int y, int w, int h) : x(x), y(y), w(w), h(h){}

	bool isValid(int minRoomSize) const
	{ return w >= minRoomSize && h >= minRoomSize;	}

	SUB_LEVEL split(bool hSplit, int splitPos)
	{
		if(hSplit)
		{
			int oldH = h;
			h = splitPos - y;

			return SUB_LEVEL(x, splitPos, w, oldH-h);
		}
		else
		{
			int oldW = w;
			w = splitPos - x;

			return SUB_LEVEL(splitPos, y, oldW-w, h);
		}
	}

	bool testSplit(bool hSplit, int splitPos, int minRoomSize)
	{
		SUB_LEVEL copyOfThis(*this);

		if(!hSplit &&
			(copyOfThis.x + copyOfThis.w <= splitPos ||
			splitPos <= copyOfThis.x ||
			splitPos - x < minRoomSize
			))
			return false;
		if(hSplit &&
			(copyOfThis.y + copyOfThis.h <= splitPos ||
			splitPos <= copyOfThis.y ||
			splitPos - y < minRoomSize))
			return false;

		auto temp = copyOfThis.split(hSplit, splitPos);

		return temp.isValid(minRoomSize) && copyOfThis.area() >= minRoomSize*minRoomSize && temp.area() >= minRoomSize*minRoomSize;
	}

	bool testSplitAll(bool hSplit, int minRoomSize)
	{
		if(!hSplit)
		{
			for(int i = x + 1; i < x + w - 1; i++)
				if(testSplit(hSplit, i, minRoomSize))
					return true;
		}
		else
		{
			for(int i = y + 1; i < y + h - 1; i++)
				if(testSplit(hSplit, i, minRoomSize))
					return true;
		}

		return false;
	}

	inline int area() const { return h*w;}
};

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
					if(room2.CollidesWith(room, 1))
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

Level::Room Level::CreateRoom(std::vector<Room>& rooms, Room::ROOM_TYPE type, sf::IntRect bounds)
{
	Room room;

	const int MIN_ROOM_W = 3;
	const int MAX_ROOM_W = bounds.width;
	const int MIN_ROOM_H = 3;
	const int MAX_ROOM_H = bounds.height;

	room.type = type;

	switch(type)
	{
	case Room::ROOM_TYPE::RECTANGULAR:
		{
			int numAttemptsToCreateRoom = 0;

			room.x = room.y = room.w = room.h = 0;

			room.w = ScionEngine::GetRandomNumber(MIN_ROOM_W, MAX_ROOM_W);
			room.h = ScionEngine::GetRandomNumber(MIN_ROOM_H, MAX_ROOM_H);			
			room.x = ScionEngine::GetRandomNumber(bounds.left, bounds.left + bounds.width - room.w );
			room.y = ScionEngine::GetRandomNumber(bounds.top, bounds.top + bounds.height - room.h);

			while(room.x + room.w > bounds.left + bounds.width)//bounds.left+1 is interesting vvvv
				room.x = ScionEngine::GetRandomNumber(bounds.left, bounds.left + bounds.width - room.w);

			while(room.y + room.h > bounds.top + bounds.height)//bounds.top+1 is interestingvvvv
				room.y = ScionEngine::GetRandomNumber(bounds.top, bounds.top + bounds.height - room.h);

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

	for(int i = 0; i < numRooms; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		Room& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
		{
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
			{
				level.SetTile(x, y, GROUND);
			}
		}

		if(createdRoom.w != 0)
		{

			for(int x = createdRoom.x - 1; x <= createdRoom.x + createdRoom.w; x++)
			{
				//attempt to place a tile above and below
				try{level.SetTile(x, createdRoom.y - 1, WALL);}
				catch(out_of_range ex){}

				try{level.SetTile(x, createdRoom.y + createdRoom.h, WALL);}
				catch(out_of_range ex){}
			}

			for(int y = createdRoom.y - 1; y <= createdRoom.y + createdRoom.h; y++)
			{
				//attempt to place a tile above and below
				try{level.SetTile(createdRoom.x - 1, y, WALL);}
				catch(out_of_range ex){}

				try{level.SetTile(createdRoom.x + createdRoom.w, y, WALL);}
				catch(out_of_range ex){}
			}

			auto randWallX = ScionEngine::GetRandomNumber(0, createdRoom.w-1);
			auto randWallY = ScionEngine::GetRandomNumber(0, createdRoom.h-1);
			auto rando = ScionEngine::GetRandomNumber(0,1);

			try{level.SetTile(createdRoom.x + randWallX, createdRoom.y-1 + rando*(createdRoom.h+1), UNUSED);}
			catch(out_of_range ex){}
		}
	}	


	for(int i = 0; i < rooms.size()-1; i++)
	{
		for(int j = i+1; j < rooms.size(); j++)
		{
			if(i == j)
				continue;
			auto&& path = level.FindPath(level.GetTile(rooms[i].x + rooms[i].w/2, rooms[i].y + rooms[i].h/2), 
				level.GetTile(rooms[j].x + rooms[j].w/2, rooms[j].y + rooms[j].h/2));

			for(auto itr = begin(path); itr != end(path); itr++)
			{
				level.SetTile(itr->x, itr->y, TILE_TYPE::CORRIDOR);
				//level.GetTile(itr->x, itr->y).color = sf::Color::Blue;
			}
		}
	}

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

		Room&& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
				level.SetTile(x, y, GROUND);

		for(int x =createdRoom.x-1; x <= createdRoom.x + createdRoom.w; x++)
		{
			try{level.SetTile(x, createdRoom.y-1, WALL);}
			catch(out_of_range ex){}

			try{level.SetTile(x, createdRoom.y+createdRoom.h, WALL);}
			catch(out_of_range ex){}
		}

		for(int y =createdRoom.y-1; y <= createdRoom.y + createdRoom.h; y++)
		{
			try{level.SetTile(createdRoom.x-1, y, WALL);}
			catch(out_of_range ex){}
			try{level.SetTile(createdRoom.x+createdRoom.w, y, WALL);}
			catch(out_of_range ex){}
		}
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

	//open up doors

	for(int i = 0; i < rooms.size(); i++)
	{
		Room& room = rooms[i];

		bool usingX = ScionEngine::GetRandomNumber(0,1);
		bool side1 = ScionEngine::GetRandomNumber(0,1);
		int xVal = ScionEngine::GetRandomNumber(room.x, room.x + room.w - 1);
		int yVal = ScionEngine::GetRandomNumber(room.y, room.y + room.h - 1);

		while(true)
		{
			if(usingX)
			{
				//pick random place on the top or bottom (not corner) as a door
				//make sure its not blocked by a wall first
				if(side1) //top
				{	//check additional north tile for 'room to move'
					try{
						if(level.GetTile(xVal, room.y-2).type == UNUSED)
							level.SetTile(xVal, room.y-1, UNUSED);
						break;
					}
					catch(out_of_range ex)
					{	side1 = false; }
				}

				if(!side1) //bottom
				{
					try{
						if(level.GetTile(xVal, room.y+room.h+1).type == UNUSED)
							level.SetTile(xVal, room.y+room.h, UNUSED);
						break;
					}
					catch(out_of_range ex)
					{	usingX = !(side1 = true);}
				}
			}

			if(!usingX)
			{
				if(side1) //left			
				{
					try{
						if(level.GetTile(room.x-2, yVal).type == UNUSED)
							level.SetTile(room.x-1, yVal, UNUSED);
						break;
					}
					catch(out_of_range ex)
					{	side1 = false;			}
				}

				if(!side1) //right
				{
					try{
						if(level.GetTile(room.x+room.w+1, yVal).type == UNUSED)
							level.SetTile(room.x+room.w, yVal, UNUSED);
						break;
					}
					catch(out_of_range ex)
					{	usingX = (side1 = true); }
				}
			}
		}
	}


	//generate corridors
	//for(int x = 1; x < level.GetWidth(); x+=2)
	//	for(int y = 1; y < level.GetWidth(); y+=2)
	//	{
	//		const auto& t = level.GetTile(x, y);

	//		if(t.type != BLANK && t.type != UNUSED)
	//			continue;

	//		level.OpenCorridor(x, y);
	//	}

	return level;
}

Level Level::CreateLevelWithDLA(LEVEL_SIZE levelSize)
{
	Level& level = Level(levelSize, levelSize);

	for(int i = 0; i < levelSize; i++)
		for(int j = 0; j < levelSize; j++)
			level.SetTile(i, j, WALL);

	int scaledSize = levelSize/10 + 1;
	int numSeed = ScionEngine::GetRandomNumber(20*scaledSize, 40*scaledSize);

	for(int s = 0; s < numSeed; s++)
	{
		int randX = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);
		int randY = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);

		int numTries = 0;

		while(level.GetTile(randX, randY).type == GROUND)
		{
			numTries++;

			randX = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);
			randY = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);

			if(numTries >= 10)
				break;
		}

		level.SetTile(randX, randY, GROUND);
	}

	static sf::Vector2i directionsCanGo[] = 
	{
		sf::Vector2i(-1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(1, 0),
		sf::Vector2i(0, -1)
	};

	int numWalkers = 10*levelSize;
	while(numWalkers > 0)
	{
		sf::Vector2i walker(ScionEngine::GetRandomNumber(1, levelSize-2), ScionEngine::GetRandomNumber(1, levelSize-2));
		sf::Vector2i oldDir;

		int numDirsTried = 0;
		while(level.GetTile(walker.x, walker.y).type != GROUND)
		{
			sf::Vector2i randDirection = directionsCanGo[ScionEngine::GetRandomNumber(0, 3)];

			while(oldDir == randDirection)
			{
				numDirsTried++;
				randDirection = directionsCanGo[ScionEngine::GetRandomNumber(0, 3)];
			}

			if(numDirsTried >= 3)
				break;

			try{
				if(level.GetTile(walker.x + randDirection.x, walker.y + randDirection.y).type == GROUND)
				{				
					level.SetTile(walker.x, walker.y, GROUND);
					break;
				}
				else
					walker += randDirection;
			}
			catch(out_of_range ex)
			{}
		}

		numWalkers--;
	}

	return level;
}

Level Level::CreateLevelWithBSP(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);
	int numRooms =  ((levelSize)*(levelSize))/(maxRoomSize*maxRoomSize);
	vector<Room> rooms;
	const int maxBSPSize = 3;

	vector<SUB_LEVEL> subdungeons;
	SUB_LEVEL* root;
	SUB_LEVEL dungeon(0, 0, levelSize, levelSize);

	auto avgSizeOfDungeonParts = [](vector<SUB_LEVEL>& parts) -> int
	{
		int sum = 0;

		for(int i = 0; i < parts.size(); i++)
			sum += parts[i].area();

		sum /= parts.size();

		return sum;
	};

	auto getRandomSplitNumber = [](double sL, double sH, bool hSplit, SUB_LEVEL& sdung) -> int
	{
		return ScionEngine::GetRandomNumber(
				(!hSplit*(sdung.x + sdung.w*sL)) + (hSplit*(sdung.y + sdung.h*sL)),
				(!hSplit*(sdung.x + sdung.w*sH)) + (hSplit*(sdung.y + sdung.h*sH)));
	};

	int smallestPartArea = dungeon.area();
	subdungeons.push_back(dungeon);

	bool splitting = true;
	while(avgSizeOfDungeonParts(subdungeons) > maxRoomSize*maxRoomSize && splitting && subdungeons.size() <= numRooms)
	{
		int oldSize = subdungeons.size();
		int numDone = 0;

		for(int i = 0; i < oldSize; i++)
		{
			auto& sd = subdungeons[i];

			bool hSplit = static_cast<bool>(ScionEngine::GetRandomNumber(0,1));
			double splitLow = 0.0;
			double splitHigh = 1.0;

			int splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, sd);

			//make sure the split wont make the resulting dungeon too small
			if(sd.testSplit(hSplit, splitPos, maxBSPSize))
			{
				subdungeons.push_back(sd.split(hSplit, splitPos));
			}
			else
			{
				bool tsahs = sd.testSplitAll(hSplit, maxBSPSize);
				bool tsanhs = sd.testSplitAll(!hSplit, maxBSPSize);

				if(!tsahs && !tsanhs)
				{
					//youre done with this one
					numDone++;
					continue;
				}
				else
				{
					if(tsahs)
					{
					}
					else if(tsanhs)
					{
tsanhs:
						hSplit = !hSplit;
					}

					//try a different splitPos
					set<int> triedSplitPos;
					triedSplitPos.insert(splitPos);

					splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, sd);

					while(!sd.testSplit(hSplit, splitPos, maxBSPSize))
					{
						splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, sd);

						int limits = !hSplit*(sd.w*(splitHigh - splitLow)) + 
							hSplit*(sd.h*(splitHigh - splitLow));

						while(triedSplitPos.find(splitPos) != triedSplitPos.end() && triedSplitPos.size() <= limits) 
						{						
							splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, sd);
						}

						if(triedSplitPos.size() >= limits)
						{
							goto tsanhs;
						}

						triedSplitPos.insert(splitPos);
					}

					subdungeons.push_back(sd.split(hSplit, splitPos));
				}
			}
		}

		splitting = (numDone != oldSize);
	}


	for(int i = 0; i < subdungeons.size(); i++)
	{
		auto& d = subdungeons[i];

		//create room contained in this subdungeon
		auto& createdRoom = CreateRoom(rooms, Room::ROOM_TYPE::RECTANGULAR, sf::IntRect(d.x, d.y, d.w, d.h));
		

		sf::Color c(ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255));

		//for(int x = d.x; x < d.x + d.w; x++)
		//	for(int y = d.y; y < d.y + d.h; y++)
		//	{
		//		level.SetTile(x, y, WALL);
		//		level.GetTile(x, y).color = c;
		//	}

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
			{
				level.SetTile(x, y, GROUND);
				//level.GetTile(x, y).color = sf::Color::White;
			}
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
		return 9999.0f;
		//if (!GetTile(s.x + (e.x - s.x), s.y).solid && !GetTile(s.x, s.y + (e.y - s.y)).solid)
		//	return 0.9f;
		//else
		//	return 140.0f;
	}
	else
		return 0;
}

list<Tile> Level::GetNeighbors(const Tile& n)
{
	//returns the neighbors of a node

	list<Tile> neighbors;

	if (n.x > 0)
	{
		neighbors.push_back(GetTile(n.x - 1, n.y));

		if (n.y > 0)
			neighbors.push_back(GetTile(n.x - 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push_back(GetTile(n.x - 1, n.y + 1));
	}

	if (n.y > 0)
		neighbors.push_back(GetTile(n.x, n.y - 1));

	if (n.x < w - 1)
	{
		neighbors.push_back(GetTile(n.x + 1, n.y));

		if (n.y > 0)
			neighbors.push_back(GetTile(n.x + 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push_back(GetTile(n.x + 1, n.y + 1));
	}

	if (n.y < h - 1)
		neighbors.push_back(GetTile(n.x, n.y + 1));

	return neighbors;
}

list<Tile>& Level::ConstructPath(std::unordered_map<Tile, Tile>& q, list<Tile>& path, const Tile& t)
{
	//traverses the map and constructs a path given a node
	if (q.find(t) != q.end())
	{
		path = ConstructPath(q, path, q[t]);
		path.push_back(t);
		return path;
	}
	else
		return path;
}

list<Tile> Level::FindPath(const Tile& start, const Tile& end)
{
	bool newGScoreBetter = false;
	unordered_set<Tile> closedSet;
	unordered_set<Tile> openSet;
	openSet.insert(start);

	std::unordered_map<Tile, Tile> navigated;
	std::unordered_map<Tile, float> gScore;
	std::unordered_map<Tile, float> hScore;
	std::map<float, list<Tile>> fScore; //easy way to use a structure to auto-sort for you

	gScore[start] = 0;
	hScore[start] = HeuristicForNode(start, end);
	fScore[gScore[start] + hScore[start]].push_back(start);

	while (openSet.size() > 0)
	{
		//take the lowest f-score node and dequeue it
		auto& lowestScore = fScore.begin()->second;
		Tile current = lowestScore.front();
		lowestScore.pop_front();

		if (lowestScore.size() == 0)
			fScore.erase(fScore.begin()); //just some maintenance to make sure you keep the lowest fscore with .First()

		if (current == end) //reached the end
		{
			list<Tile> path;
			path.push_back(start);
			return ConstructPath(navigated, path, current);
		}

		openSet.erase(current); //remove current from open
		closedSet.insert(current); //put into closed

		auto neighbors = GetNeighbors(current);
		for(auto neighbor1 = std::begin(neighbors); neighbor1 != std::end(neighbors); neighbor1++)
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

				fScore[gScore[neighbor] + hScore[neighbor]].push_back(neighbor);
			}
		}
	}

	//failed
	return list<Tile>();
}
