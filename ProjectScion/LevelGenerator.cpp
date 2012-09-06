#include "LevelGenerator.h"
#include "ScionEngine.h"
#include "LevelPartition.h"
#include "Direction.h"

#include <noise/noise.h>
#include "noiseutils.h"

#include <Windows.h>

using namespace noise;
using namespace std;

vector<Tile*> LevelGenerator::corridorSeeds;
unordered_map<Tile*, shared_ptr<CorridorBranch>> LevelGenerator::corridorMap;
unordered_map<Tile*, shared_ptr<RoomBranch>> LevelGenerator::roomMap;

ROOM LevelGenerator::CreateRoom(vector<ROOM>& rooms, ROOM::TYPE type, ROOM::SIZE size, int levelWidth, int levelHeight)
{
	ROOM room;

	const int MIN_ROOM_W = 3;
	const int MAX_ROOM_W = size;
	const int MIN_ROOM_H = 3;
	const int MAX_ROOM_H = size;

	room.type = type;

	switch(type)
	{
	case ROOM::TYPE::RECTANGULAR:
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

				for_each(begin(rooms), end(rooms), [&](ROOM& room2)
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

			break;
		}
	case ROOM::TYPE::STARBURST:
		break;
	case ROOM::TYPE::ROTATED:
		break;
	case ROOM::TYPE::CIRCULAR:
		break;
	}

	rooms.push_back(room);

	return room;
}

ROOM LevelGenerator::CreateRoom(vector<ROOM>& rooms, ROOM::TYPE type, sf::IntRect bounds)
{
	ROOM room;

	const int MIN_ROOM_W = 3;
	const int MAX_ROOM_W = bounds.width; //take out the -2 for largely connected rooms
	const int MIN_ROOM_H = 3;
	const int MAX_ROOM_H = bounds.height;

	room.type = type;

	switch(type)
	{
	case ROOM::TYPE::RECTANGULAR:
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
	case ROOM::TYPE::STARBURST:
		break;
	case ROOM::TYPE::ROTATED:
		break;
	case ROOM::TYPE::CIRCULAR:
		break;
	}

	rooms.push_back(room);

	return room;
}

void LevelGenerator::GenerateRooms(Level& level, vector<ROOM>& rooms, int numRooms, ROOM::SIZE maxRoomSize, int levelSize)
{	
	for(int i = 0; i < numRooms; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		ROOM&& createdRoom = CreateRoom(rooms, ROOM::TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);
		
		if(createdRoom.w > 0)
		{
			//success
			vector<Tile*> roomTiles;
			int numDoors = 0, maxDoors = 1 + createdRoom.w*createdRoom.h/35;
			
			for(int x = createdRoom.x-1; x <= createdRoom.x + createdRoom.w; x++)
				for(int y = createdRoom.y-1; y <= createdRoom.y + createdRoom.h; y++)
				{
					if(x != 0 && y != 0 && x != level.GetWidth()-1 && y != level.GetHeight()-1)
					{
						if(x == createdRoom.x - 1 ||
							x == createdRoom.x + createdRoom.w ||
							y == createdRoom.y - 1 ||
							y == createdRoom.y + createdRoom.h)
						{
							level.SetTile(x, y, Tile::WALL);
						
							roomTiles.push_back(&level.GetTile(x, y));
						}
						else
							level.SetTile(x, y, Tile::GROUND);
					}
				}				

			//place doors
			while(numDoors < maxDoors)
			{
				bool whichSide = ScionEngine::GetRandomNumber(0, 1); //false = top/left, true = bot/right
				bool topOrLeft = ScionEngine::GetRandomNumber(0, 1); //false = horizontal, true = vertical
				int randX = ( topOrLeft*whichSide*(createdRoom.w+1)) + (!topOrLeft)*ScionEngine::GetRandomNumber(0, createdRoom.w+1);
				int randY = (!topOrLeft*whichSide*(createdRoom.h+1)) + ( topOrLeft)*ScionEngine::GetRandomNumber(0, createdRoom.h+1);
				
				int wallX = createdRoom.x-1+randX;
				int wallY = createdRoom.y-1+randY;

				while((wallX % 2 == 0 && wallY % 2 == 0) ||
					(wallX == 0 || wallX == level.GetWidth()-1 ||
					wallY == 0 || wallY == level.GetHeight()-1))
				{			
					whichSide = ScionEngine::GetRandomNumber(0, 1); //false = top/left, true = bot/right
					topOrLeft = ScionEngine::GetRandomNumber(0, 1); //false = horizontal, true = vertical
					randX = ( topOrLeft*whichSide*(createdRoom.w+1)) + (!topOrLeft)*ScionEngine::GetRandomNumber(0, createdRoom.w+1);
					randY = (!topOrLeft*whichSide*(createdRoom.h+1)) + ( topOrLeft)*ScionEngine::GetRandomNumber(0, createdRoom.h+1);

					wallX = createdRoom.x-1+randX;
					wallY = createdRoom.y-1+randY;
				}

				level.SetTile(wallX, wallY, Tile::BLANK, true);
				numDoors++;
			}
			//for(int x =createdRoom.x-1; x <= createdRoom.x + createdRoom.w; x++)
			//{
			//	try{
			//		level.SetTile(x, createdRoom.y-1, Tile::WALL);
			//		roomTiles.push_back(&level.GetTile(x, createdRoom.y-1));
			//	} catch(out_of_range ex){}

			//	try{
			//		level.SetTile(x, createdRoom.y+createdRoom.h, Tile::WALL);
			//		roomTiles.push_back(&level.GetTile(x, createdRoom.y+createdRoom.h));
			//	} catch(out_of_range ex){}
			//}

			//for(int y =createdRoom.y-1; y <= createdRoom.y + createdRoom.h; y++)
			//{
			//	try{
			//		level.SetTile(createdRoom.x-1, y, Tile::WALL);
			//		roomTiles.push_back(&level.GetTile(createdRoom.x-1, y));
			//	} catch(out_of_range ex){}
			//	try{
			//		level.SetTile(createdRoom.x+createdRoom.w, y, Tile::WALL);
			//		roomTiles.push_back(&level.GetTile(createdRoom.x+createdRoom.w, y));
			//	} catch(out_of_range ex){}
			//}

			auto roomObj = make_shared<RoomBranch>(roomTiles);
			
			for(int i = 0; i < roomTiles.size(); i++)
				if(roomTiles[i]->entrance)
					roomMap[roomTiles[i]] = roomObj;
		}
	}
}

void LevelGenerator::WallUpRooms(Level& level, vector<ROOM>& rooms)
{
	for(int i = 0; i < rooms.size(); i++)
	{
	}
}

void LevelGenerator::OpenDoorsOnRooms(Level& level, vector<ROOM>& rooms)
{
	for(int i = 0; i < rooms.size(); i++)
	{
		ROOM& room = rooms[i];

		bool usingX = ScionEngine::GetRandomNumber(0,1);
		bool side1 = ScionEngine::GetRandomNumber(0,1);
		int xVal = ScionEngine::GetRandomNumber(room.x, room.x + room.w - 1);
		int yVal = ScionEngine::GetRandomNumber(room.y, room.y + room.h - 1);

		while(xVal % 2 == 0)
			xVal = ScionEngine::GetRandomNumber(room.x, room.x + room.w - 1);
		while(yVal % 2 == 0)
			yVal = ScionEngine::GetRandomNumber(room.y, room.y + room.h - 1);

		while(true)
		{
			if(usingX)
			{
				//pick random place on the top or bottom (not corner) as a door
				//make sure its not blocked by a wall first
				if(side1) //top
				{	//check additional north tile for 'room to move'
					try{
						if(level.GetTile(xVal, room.y-2).type == Tile::UNUSED)
							level.SetTile(xVal, room.y-1, Tile::BLANK, true);
						room.doorX = xVal;
						room.doorY = room.y - 1;
						break;
					}
					catch(out_of_range ex)
					{	side1 = false; }
				}

				if(!side1) //bottom
				{
					try{
						if(level.GetTile(xVal, room.y+room.h+1).type == Tile::UNUSED)
							level.SetTile(xVal, room.y+room.h, Tile::BLANK, true);
						room.doorX = xVal;
						room.doorY = room.y+room.h;
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
						if(level.GetTile(room.x-2, yVal).type == Tile::UNUSED)
							level.SetTile(room.x-1, yVal, Tile::BLANK, true);
						room.doorX = room.x-1;
						room.doorY = yVal;
						break;
					}
					catch(out_of_range ex)
					{	side1 = false;			}
				}

				if(!side1) //right
				{
					try{
						if(level.GetTile(room.x+room.w+1, yVal).type == Tile::UNUSED)
							level.SetTile(room.x+room.w, yVal, Tile::BLANK, true);
						room.doorX = room.x+room.w;
						room.doorY = yVal;
						break;
					}
					catch(out_of_range ex)
					{	usingX = (side1 = true); }
				}
			}
		}
	}
}

void LevelGenerator::GenerateCorridors(Level& level)
{
	Direction::InitializeDirections();

	for(int i = 1; i < level.GetWidth(); i++)
	{
		for(int j = 0; j < level.GetHeight(); j++)
		{
			if(i%2 != 0 && j%2 != 0)
			{
				if(level.GetTile(i, j).type == Tile::UNUSED)
				{
					OpenCorridor(level, i, j, nullptr);
					corridorSeeds.push_back(&level.GetTile(i, j));
				}
			}
		}
	}
}

void LevelGenerator::OpenCorridor(Level& level, int x, int y, CorridorBranch* parent, string lastDir)
{
	deque<string> dirs = move(Direction::GetKeys());
	if(lastDir != "")
		dirs.push_front(lastDir);

	for(int d = 0; d < dirs.size(); d++)
	{
		string dKey = dirs[d];

		vector<Tile*> tiles;
		tiles.push_back(&level.GetTile(x, y));

		bool failed = false, corientrance = false;

		for(int step = 1; step <= 2; step++)
		{
			sf::Vector2i jump = Direction::DIRECTIONS[dKey];

			if(tiles[step-1] != nullptr && !failed)
			{
				try{tiles.push_back(&level.GetTile(tiles[step-1]->x + jump.x, tiles[step-1]->y + jump.y));}
				catch(out_of_range ex)
				{ tiles.push_back(nullptr);}

				if(tiles[step] == nullptr || tiles[step]->type != Tile::UNUSED)
					failed = true;

				if(tiles[step] != nullptr && tiles[step]->entrance)
				{
					corientrance = failed = true;
					break;
				}
			}
			else
			{
				failed = true;
			}
		}

		if(!failed)
		{
			auto corridorObj = make_shared<CorridorBranch>(tiles);

			for(int b = 0; b < tiles.size(); b++)
			{
				level.SetTile(tiles[b]->x, tiles[b]->y, Tile::GROUND);
				corridorMap[tiles[b]] = corridorObj;
			}

			if(parent != nullptr)
				parent->children.push_back(corridorObj.get());

			OpenCorridor(level, tiles[tiles.size()-1]->x, tiles[tiles.size()-1]->y, corridorObj.get(), dKey);
		}

		if(corientrance)
		{
			auto corridorObj = make_shared<CorridorBranch>(tiles);

			for(int b = 0; b < tiles.size(); b++)
			{
				level.SetTile(tiles[b]->x, tiles[b]->y, Tile::GROUND);
				corridorMap[tiles[b]] = corridorObj;
			}

			if(parent != nullptr)
				parent->children.push_back(roomMap[tiles[tiles.size()-1]].get());
		}
	}
}

void LevelGenerator::TrimTree(Level& level)
{
	for(int i = 0; i < corridorSeeds.size(); i++)
		CheckBranch(level, corridorMap[corridorSeeds[i]].get());
}

//if branch does not have room, remove reference
bool LevelGenerator::CheckBranch(Level& level, Branch* branch)
{
	if(branch == nullptr)
		return false;

	bool room = false;

	for(int i = 0; i < branch->children.size(); i++)
	{
		auto c = branch->children[i];

		bool tRoom = CheckBranch(level, c);
		//if no room, remove corr

		if(!tRoom || branch->children.size() > level.GetWidth()/2)
		{
			//if(c)
				for(int j = 0; j < c->tiles.size(); j++)
					level.SetTile(c->tiles[j]->x, c->tiles[j]->y, Tile::UNUSED);
		}
		else
			if(!room) room = true;
	}

	if(room)
	{
		auto lastTile = branch->tiles[branch->tiles.size()-1];
		level.SetTile(lastTile->x, lastTile->y, Tile::GROUND);
	}
	else
	{
		room = branch->Type() == Branch::ROOM;
	}

	return room;
}

Level LevelGenerator::CreateLevelWithRooms2(Level::SIZE levelSize, ROOM::SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);

	int numRooms =  ((levelSize-2)*(levelSize-2))/(maxRoomSize*maxRoomSize);

	//fun time - create numRooms amount of 'rooms'

	vector<ROOM> rooms;
	rooms.reserve(numRooms);

	for(int i = 0; i < numRooms; i++)
	{
		int randNum = ScionEngine::GetRandomNumber(1, 10);		

		ROOM& createdRoom = CreateRoom(rooms, ROOM::TYPE::RECTANGULAR, maxRoomSize, levelSize, levelSize);

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
		{
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
			{
				level.SetTile(x, y, Tile::GROUND);
			}
		}

		//if(createdRoom.w != 0)
		//{

		//	for(int x = createdRoom.x - 1; x <= createdRoom.x + createdRoom.w; x++)
		//	{
		//		//attempt to place a tile above and below
		//		try{level.SetTile(x, createdRoom.y - 1, Tile::WALL);}
		//		catch(out_of_range ex){}

		//		try{level.SetTile(x, createdRoom.y + createdRoom.h, Tile::WALL);}
		//		catch(out_of_range ex){}
		//	}

		//	for(int y = createdRoom.y - 1; y <= createdRoom.y + createdRoom.h; y++)
		//	{
		//		//attempt to place a tile above and below
		//		try{level.SetTile(createdRoom.x - 1, y, Tile::WALL);}
		//		catch(out_of_range ex){}

		//		try{level.SetTile(createdRoom.x + createdRoom.w, y, Tile::WALL);}
		//		catch(out_of_range ex){}
		//	}

		//	auto randWallX = ScionEngine::GetRandomNumber(0, createdRoom.w-1);
		//	auto randWallY = ScionEngine::GetRandomNumber(0, createdRoom.h-1);
		//	auto rando = ScionEngine::GetRandomNumber(0,1);

		//	try{level.SetTile(createdRoom.x + randWallX, createdRoom.y-1 + rando*(createdRoom.h+1), Tile::UNUSED);}
		//	catch(out_of_range ex){}
		//}
	}	


	//for(int i = 0; i < rooms.size()-1; i++)
	//{
	//	for(int j = i+1; j < rooms.size(); j++)
	//	{
	//		if(i == j)
	//			continue;
	//		auto&& path = level.FindPath(level.GetTile(rooms[i].x + rooms[i].w/2, rooms[i].y + rooms[i].h/2), 
	//			level.GetTile(rooms[j].x + rooms[j].w/2, rooms[j].y + rooms[j].h/2));

	//		for(auto itr = begin(path); itr != end(path); itr++)
	//		{
	//			level.SetTile(itr->x, itr->y, Tile::TYPE::Tile::CORRIDOR);
	//			//level.GetTile(itr->x, itr->y).color = sf::Color::Blue;
	//		}
	//	}
	//}

	return level;
}

Level LevelGenerator::CreateLevelWithRooms1(Level::SIZE levelSize, ROOM::SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);
	
	for(int i = 0; i < levelSize; i++)
	{
		level.SetTile(i, 0, Tile::WALL);
		level.SetTile(0, i, Tile::WALL);
		level.SetTile(i, levelSize-1, Tile::WALL);
		level.SetTile(levelSize-1, i, Tile::WALL);
	}

	int numRooms =  ((levelSize)*(levelSize))/(maxRoomSize*maxRoomSize);

	//fun time - create numRooms amount of 'rooms'

	vector<ROOM> rooms;
	rooms.reserve(numRooms);

	GenerateRooms(level, rooms, numRooms, maxRoomSize, levelSize);


	//WallUpRooms(level, rooms);

	//open up doors
	//OpenDoorsOnRooms(level, rooms);

	GenerateCorridors(level);
	//generate corridors
	//for(int x = 1; x < level.GetWidth(); x+=2)
	//	for(int y = 1; y < level.GetWidth		(); y+=2)
	//	{
	//		const auto& t = level.GetTile(x, y);

	//		if(t.type != Tile::BLANK && t.type != Tile::UNUSED)
	//			continue;

	//		level.OpenCorridor(x, y);
	//	}

	TrimTree(level);
	Cleanup();
	return level;
}

Level LevelGenerator::CreateLevelWithDLA(Level::SIZE levelSize)
{
	Level& level = Level(levelSize, levelSize);

	for(int i = 0; i < levelSize; i++)
		for(int j = 0; j < levelSize; j++)
			level.SetTile(i, j, Tile::WALL);

	int scaledSize = levelSize/10 + 1;
	int numSeed = ScionEngine::GetRandomNumber(20*scaledSize, 40*scaledSize);

	for(int s = 0; s < numSeed; s++)
	{
		int randX = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);
		int randY = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);

		int numTries = 0;

		while(level.GetTile(randX, randY).type == Tile::GROUND)
		{
			numTries++;

			randX = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);
			randY = levelSize/2 + ScionEngine::GetRandomNumber(-scaledSize, scaledSize);

			if(numTries >= 10)
				break;
		}

		level.SetTile(randX, randY, Tile::GROUND);
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
		while(level.GetTile(walker.x, walker.y).type != Tile::GROUND)
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
				if(level.GetTile(walker.x + randDirection.x, walker.y + randDirection.y).type == Tile::GROUND)
				{				
					level.SetTile(walker.x, walker.y, Tile::GROUND);
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

Level LevelGenerator::CreateLevelWithBSP(Level::SIZE levelSize, ROOM::SIZE maxRoomSize)
{
	Level& level = Level(levelSize, levelSize);
	int numRooms =  ((levelSize)*(levelSize))/(maxRoomSize*maxRoomSize);
	vector<ROOM> rooms;
	const int maxBSPSize = 5;

	vector<LEVEL_PARTITION> subdungeons, allSD;
	LEVEL_PARTITION dungeon(0, 0, levelSize, levelSize);
	dungeon.id = allSD.size();

	auto avgSizeOfDungeonParts = [](vector<LEVEL_PARTITION>& parts) -> int
	{
		int sum = 0;

		for(int i = 0; i < parts.size(); i++)
			sum += parts[i].area();

		sum /= parts.size();

		return sum;
	};

	auto getRandomSplitNumber = [](double sL, double sH, bool hSplit, LEVEL_PARTITION& sdung) -> int
	{
		return ScionEngine::GetRandomNumber(
				(!hSplit*(sdung.x + sdung.w*sL)) + (hSplit*(sdung.y + sdung.h*sL)),
				(!hSplit*(sdung.x + sdung.w*sH)) + (hSplit*(sdung.y + sdung.h*sH)));
	};

	int smallestPartArea = dungeon.area();
	subdungeons.push_back(dungeon);
	allSD.push_back(dungeon);

	bool splitting = true;
	while(avgSizeOfDungeonParts(subdungeons) > maxRoomSize*maxRoomSize && splitting && subdungeons.size() <= numRooms)
	{
		int oldSize = subdungeons.size();
		int numDone = 0;

		for(int i = 0; i < oldSize; i++)
		{
			bool hSplit = static_cast<bool>(ScionEngine::GetRandomNumber(0,1));
			double splitLow = 0.0;
			double splitHigh = 1.0;

			int splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, subdungeons[i]);

			//make sure the split wont make the resulting dungeon too small
			if(subdungeons[i].testSplit(hSplit, splitPos, maxBSPSize))
			{
				LEVEL_PARTITION a, b;
				subdungeons[i].split(hSplit, splitPos, a, b);				
				
				a.parent = subdungeons[i].id;
				b.parent = subdungeons[i].id;

				a.id = allSD.size();
				allSD.push_back(a);

				b.id = allSD.size();
				allSD.push_back(b);

				subdungeons.push_back(a);
				subdungeons.push_back(b);
				
				allSD[subdungeons[i].id].left = a.id;
				allSD[subdungeons[i].id].right = b.id;

				subdungeons.erase(subdungeons.begin() + i);
			}
			else
			{
				bool tsahs = subdungeons[i].testSplitAll(hSplit, maxBSPSize);
				bool tsanhs = subdungeons[i].testSplitAll(!hSplit, maxBSPSize);

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

					splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, subdungeons[i]);

					while(!subdungeons[i].testSplit(hSplit, splitPos, maxBSPSize))
					{
						splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, subdungeons[i]);

						int limits = !hSplit*(subdungeons[i].w*(splitHigh - splitLow)) + 
							hSplit*(subdungeons[i].h*(splitHigh - splitLow));

						while(triedSplitPos.find(splitPos) != triedSplitPos.end() && triedSplitPos.size() <= limits) 
						{						
							splitPos = getRandomSplitNumber(splitLow, splitHigh, hSplit, subdungeons[i]);
						}

						if(triedSplitPos.size() >= limits)
						{
							goto tsanhs;
						}

						triedSplitPos.insert(splitPos);
					}

					LEVEL_PARTITION a, b;
					subdungeons[i].split(hSplit, splitPos, a, b);				
				
					a.parent = subdungeons[i].id;
					b.parent = subdungeons[i].id;

					a.id = allSD.size();
					allSD.push_back(a);

					b.id = allSD.size();
					allSD.push_back(b);

					subdungeons.push_back(a);
					subdungeons.push_back(b);
				
					allSD[subdungeons[i].id].left = a.id;
					allSD[subdungeons[i].id].right = b.id;

					subdungeons.erase(subdungeons.begin() + i);
				}
			}
		}

		splitting = (numDone != oldSize);
	}


	for(int i = 0; i < subdungeons.size(); i++)
	{
		auto& d = subdungeons[i];

		//create room contained in this subdungeon
		auto& createdRoom = CreateRoom(rooms, ROOM::TYPE::RECTANGULAR, sf::IntRect(d.x, d.y, d.w, d.h));

		//while the room is touching another room, try a new room
		bool collidesWithOtherRoom = false;

		for(int f = 0; f < rooms.size() - 1; f++)
			if(createdRoom.CollidesWith(rooms[f], 1, 0))
				collidesWithOtherRoom = true;

		while(collidesWithOtherRoom)
		{
			rooms.pop_back();
			createdRoom = CreateRoom(rooms, ROOM::TYPE::RECTANGULAR, sf::IntRect(d.x, d.y, d.w, d.h));

			//while the room is touching another room, try a new room
			collidesWithOtherRoom = false;

			for(int i = 0; i < rooms.size() - 1; i++)
				if(createdRoom.CollidesWith(rooms[i], 1, 0))
					collidesWithOtherRoom = true;
		}

		allSD[subdungeons[i].id].room = createdRoom;
		

		

		sf::Color c(ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255), ScionEngine::GetRandomNumber(0, 255));

		//for(int x = d.x; x < d.x + d.w; x++)
		//	for(int y = d.y; y < d.y + d.h; y++)
		//	{
		//		level.SetTile(x, y, Tile::WALL);
		//		level.GetTile(x, y).color = c;
		//	}

		for(int x = createdRoom.x; x < createdRoom.x + createdRoom.w; x++)
			for(int y = createdRoom.y; y < createdRoom.y + createdRoom.h; y++)
			{
				level.SetTile(x, y, Tile::GROUND);
				//level.GetTile(x, y).color = sf::Color::White;
			}

		//for(int x =createdRoom.x-1; x <= createdRoom.x + createdRoom.w; x++)
		//{
		//	try{level.SetTile(x, createdRoom.y-1, Tile::WALL);}
		//	catch(out_of_range ex){}

		//	try{level.SetTile(x, createdRoom.y+createdRoom.h, Tile::WALL);}
		//	catch(out_of_range ex){}
		//}

		//for(int y =createdRoom.y-1; y <= createdRoom.y + createdRoom.h; y++)
		//{
		//	try{level.SetTile(createdRoom.x-1, y, Tile::WALL);}
		//	catch(out_of_range ex){}
		//	try{level.SetTile(createdRoom.x+createdRoom.w, y, Tile::WALL);}
		//	catch(out_of_range ex){}
		//}
	}
	
	//for(int x =0; x < levelSize; x++)
	//{
	//	level.SetTile(x, 0, Tile::WALL);
	//	level.SetTile(x, levelSize-1, Tile::WALL);
	//}

	//for(int y = 0; y < levelSize; y++)
	//{
	//	level.SetTile(0, y, Tile::WALL);
	//	level.SetTile(levelSize-1, y, Tile::WALL);
	//}

	//open up doors

	for(int i = 0; i < rooms.size(); i++)
	{
		ROOM& room = rooms[i];

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
						if(level.GetTile(xVal, room.y-2).type == Tile::UNUSED);
							level.SetTile(xVal, room.y-1, Tile::UNUSED);
						room.doorX = xVal;
						room.doorY = room.y-1;
						break;
					}
					catch(out_of_range ex)
					{	side1 = false; }
				}

				if(!side1) //bottom
				{
					try{
						if(level.GetTile(xVal, room.y+room.h+1).type == Tile::UNUSED);
							level.SetTile(xVal, room.y+room.h, Tile::UNUSED);
						room.doorX = xVal;
						room.doorY = room.y+room.h;
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
						if(level.GetTile(room.x-2, yVal).type == Tile::UNUSED);
							level.SetTile(room.x-1, yVal, Tile::UNUSED);
						room.doorX = room.x-1;
						room.doorY = yVal;
						break;
					}
					catch(out_of_range ex)
					{	side1 = false;			}
				}

				if(!side1) //right
				{
					try{
						if(level.GetTile(room.x+room.w+1, yVal).type == Tile::UNUSED);
							level.SetTile(room.x+room.w, yVal, Tile::UNUSED);
						room.doorX = room.x+room.w;
						room.doorY = yVal;
						break;
					}
					catch(out_of_range ex)
					{	usingX = (side1 = true); }
				}
			}
		}
	}

	//rooms are set now connect them
	vector<LEVEL_PARTITION> leaves;

	for_each(begin(allSD), end(allSD), [&](LEVEL_PARTITION& lvl)
	{
		if(lvl.left == lvl.right && lvl.left == -1)
		{
			//leaf
			if(find(begin(leaves), end(leaves), lvl) == end(leaves))
				leaves.push_back(allSD[lvl.parent]);
		}
	});
	
	for(auto leaf = begin(leaves); leaf != end(leaves);)
	{
		auto left = allSD[leaf->left];
		auto right = allSD[leaf->right];
		auto leftroom = left.room;
		auto rightroom = right.room;

		if(left.left == left.right && right.left == right.right && right.left == left.left && left.left == -1)
			leaf++;
		else
			leaf = leaves.erase(leaf);
	}

	vector<LEVEL_PARTITION> seenLevels; //log(n) existence check

	auto itr = begin(leaves);
	while(itr != end(leaves))
	{
		if(find(begin(seenLevels), end(seenLevels), *itr) != end(seenLevels)) //seen? erase it
			itr = leaves.erase(itr); //itr now points to next element
		else
		{
			seenLevels.push_back(*itr);
			itr++;
		}
	}


	//connect leaves
	for(auto leaf = begin(leaves); leaf != end(leaves); leaf++)
	{
		auto left = allSD[leaf->left];
		auto right = allSD[leaf->right];
		////connect leave[i].left to leave[i].right
		//auto left = allSD[leaf->left];
		//auto right = allSD[leaf->right];

		////see which 'quadrant' right lies in
		//ROOM cT, cR, cD, cL;
		//auto collisionTop = left.room.GetCollisionRect(right.room, sf::IntRect(0, 9999, 0, 0), sf::IntRect(), &cT);
		//auto collisionRight = left.room.GetCollisionRect(right.room, sf::IntRect(0, 0, 9999, 0), sf::IntRect(), &cR);
		//auto collisionDown = left.room.GetCollisionRect(right.room, sf::IntRect(0, 0, 0, 9999), sf::IntRect(), &cD);
		//auto collisionLeft = left.room.GetCollisionRect(right.room, sf::IntRect(9999, 0, 0, 0), sf::IntRect(), &cL);

		//if(collisionTop || collisionDown)
		//{
		//	//pick a random y within [0, collisionR.h)

		//	int randomX = (collisionTop ? cT.x + ScionEngine::GetRandomNumber(0, cT.w-1) : cD.x + ScionEngine::GetRandomNumber(0, cD.w-1));
		//	//connect the rooms at this randomY
		//	
		//	for(int i = left.room.y; ; i += min(1, max(right.room.y - left.room.y, -1)))
		//	{
		//		level.SetTile(randomX, i, Tile::GROUND);

		//		if(i == right.room.y || i == right.room.y + right.room.h - 1)
		//			break;
		//	}
		//}


		//if(collisionRight || collisionLeft)
		//{
		//	//pick a random x within [0, collisionR.w)

		//	int randomY = (collisionRight ? cR.y + ScionEngine::GetRandomNumber(0, cR.h-1) : cL.y + ScionEngine::GetRandomNumber(0, cL.h-1));
		//	//connect the rooms at this randomY
		//	
		//	for(int i = left.room.x; ; i += min(1, max(right.room.x - left.room.x, -1)))
		//	{
		//		level.SetTile(i, randomY, Tile::GROUND);

		//		if(i == right.room.x || i == right.room.x + right.room.w - 1)
		//			break;
		//	}
		//}
		//pick a random point in each dungeon, connect
		sf::Vector2i leftC(left.room.x + ScionEngine::GetRandomNumber(0, left.room.w -1), left.room.y + ScionEngine::GetRandomNumber(0, left.room.h -1));
		sf::Vector2i rightC(right.room.x + ScionEngine::GetRandomNumber(0, right.room.w -1), right.room.y + ScionEngine::GetRandomNumber(0, right.room.h -1));
		
		//auto path = level.FindPath(&level.GetTile(0, 0), &level.GetTile(levelSize-1, levelSize-1));
		
		//for(auto itr = path.begin(); itr != path.end(); itr++)
		//	level.SetTile((*itr)->x, (*itr)->y, Tile::WALL);
	}	

	return level;
}

Level LevelGenerator::CreateLevelWithPerlinNoise(int levelWidth, int levelHeight)
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
				level.SetTile(x, y, Tile::GROUND);
		}

		return level;
}

Level LevelGenerator::CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight)
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
			Tile t = Tile::DefaultTiles().at(Tile::GROUND);
			t.color = sf::Color(col->red, col->green, col->blue, col->alpha);
			level.SetTile(x, y, t);
		}

		return level;
}

Level LevelGenerator::CreateLevelWithCA(int levelWidth, int levelHeight)
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
		level.SetTile(ScionEngine::GetRandomNumber(0, levelWidth-1), ScionEngine::GetRandomNumber(0, levelHeight-1), Tile::GROUND);

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
					level.SetTile(x, y, Tile::GROUND);
				}
				else if(numSolid < 4)
				{
					level.SetTile(x, y, Tile::BLANK);
				}
			}
	}

	return level;
}