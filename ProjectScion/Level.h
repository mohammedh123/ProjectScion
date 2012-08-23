#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "Tile.h"
#include "Entity.h"
#include "Camera.h"
#include <SFML\Graphics.hpp>

class Level
{
private:
	Camera camera;

	std::vector<std::vector<Tile>> map;
	std::vector<Entity*> objects;

	int w;
	int h;

	void SetDimensions(int w, int h);
public:
	enum LEVEL_SIZE
	{
		FINE = 23,
		DIMINUTIVE = 29,
		TINY = 35,
		SMALL = 45,
		MEDIUM = 57,
		LARGE = 73,
		HUGE = 101,
		GARGANTUAN = 127,
		COLOSSAL = 161
	};

	struct Room
	{
		enum ROOM_SIZE
		{
			//value is max-size
			SMALL = 5,
			MEDIUM = 11,
			LARGE = 11,
			HUGE = 17,
			GARGANTUAN = 23,
			COLOSSAL = 29
		};

		enum ROOM_TYPE
		{
			RECTANGULAR,
			STARBURST,
			CIRCULAR,
			ROTATED
		};

		int x, y, w, h;
		ROOM_TYPE type;

		inline bool CollidesWith(Room& r, int border = 0) const
		{
			// -    0    1    2    3
			// 0	.    .    .    .
			// 1	.    b(a) a    .
			// 2	.    a    a    .
			// 3	.    .    .    .
			// x = 1
			// y = 1
			// w = 1
			// h = 1
			// r.x = 1
			// r.y = 1
			// r.w = 2
			// r.h = 2
			// (1 > 2) = false
			// (3 < 1) = false
			// (1 > 2) = false
			// (3 < 1) = false
			// true


			return !(r.x - border >= x + w + border ||
				r.x + r.w + border <= x  - border ||
				r.y - border >= y + h + border ||
				r.y + r.h + border <= y - border);
		}
	};

	Level() : camera(800, 600, 0.2f) {}
	Level(int widthInTiles, int heightInTiles);
	~Level();
	
	inline void SetTile(int x, int y, Tile& tile)
	{
		map[y][x] = tile;
	}

	inline void SetTile(int x, int y, TILE_TYPE type)
	{
		auto& tile = Tile::DefaultTiles().at(type);
		map[y][x].baseSprite = tile.baseSprite;
		map[y][x].solid = tile.solid;
		map[y][x].type = type;
		map[y][x].color = sf::Color::White;
	}

	inline Tile& GetTile(int x, int y)
	{
		return map.at(y).at(x);
	}

	void LoadLevel();

	inline Camera& GetCamera() {return camera;}
	inline int GetWidth() const {return w;}
	inline int GetHeight() const {return h;}
	
	static Level CreateLevelWithRooms1(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize);
	static Level CreateLevelWithRooms2(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize);
	static Level CreateLevelWithPerlinNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithCA(int levelWidth, int levelHeight);

	void Draw(sf::RenderWindow* window);
	void OpenCorridor(int x, int y, sf::Vector2i direction = sf::Vector2i());
private:	
	bool OpenTunnel(int x, int y, sf::Vector2i direction = sf::Vector2i());
	bool IsTunnelValid(int midX, int midY, int nextX, int nextY);
	bool CreateTunnel(int thisX, int thisY, int nextX, int nextY);

	static Level::Room CreateRoom(std::vector<Room>& rooms, Room::ROOM_TYPE type, Room::ROOM_SIZE size, int levelWidth, int levelHeight);
	
	static float HeuristicForNode(const Tile& start, const Tile& end);
	float GetDistance(const Tile& s, const Tile& e);
	std::queue<Tile> GetNeighbors(const Tile& n);
	static std::queue<Tile>& Level::ConstructPath(std::unordered_map<Tile, Tile>& q, std::queue<Tile>& path, const Tile& t);
	std::queue<Tile> Level::FindPath(const Tile& start, const Tile& end);
};

#endif