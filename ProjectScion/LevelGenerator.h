#ifndef _LEVELGENERATOR_H
#define _LEVELGENERATOR_H

#include "Level.h"
#include "Room.h"
#include "CorridorBranch.h"
#include "RoomBranch.h"

#include <SFML\Graphics.hpp>

#include <vector>
#include <map>

//https://sourceforge.net/p/dungeongen/

class LevelGenerator
{	
	static std::vector<Tile*> corridorSeeds;
	//LevelGenerator is reponsible for destroying these Branch*
	static std::unordered_map<Tile*, std::shared_ptr<CorridorBranch>> corridorMap;
	static std::unordered_map<Tile*, std::shared_ptr<RoomBranch>> roomMap;

	static void GenerateRooms(Level& level, std::vector<ROOM>& rooms, int numRooms, ROOM::SIZE maxRoomSize, int levelSize);
	static void WallUpRooms(Level& level, std::vector<ROOM>& rooms);
	static void OpenDoorsOnRooms(Level& level, std::vector<ROOM>& rooms);
	static void GenerateCorridors(Level& level);
	static void OpenCorridor(Level& level, int x, int y, CorridorBranch* parent);

	static ROOM CreateRoom(std::vector<ROOM>& rooms, ROOM::TYPE type, ROOM::SIZE size, int levelWidth, int levelHeight);
	static ROOM CreateRoom(std::vector<ROOM>& rooms, ROOM::TYPE type, sf::IntRect bounds);

	static void TrimTree(Level& level);
	static bool CheckBranch(Level& level, Branch* branch);
public:
	~LevelGenerator()
	{
		Cleanup();
	}

	inline static void Cleanup()
	{
		roomMap.clear();
		corridorMap.clear();
		corridorSeeds.clear();
	}

	static Level CreateLevelWithRooms1(Level::SIZE levelSize, ROOM::SIZE maxRoomSize);
	static Level CreateLevelWithRooms2(Level::SIZE levelSize, ROOM::SIZE maxRoomSize);
	static Level CreateLevelWithDLA(Level::SIZE levelSize);
	static Level CreateLevelWithBSP(Level::SIZE levelSize, ROOM::SIZE maxRoomSize);
	static Level CreateLevelWithPerlinNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithCA(int levelWidth, int levelHeight);
};

#endif