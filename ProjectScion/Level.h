#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include <memory>

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

		inline bool CollidesWith(Room& r) const
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


			return !(r.x > x + w ||
				r.x + r.w < x ||
				r.y > y + h ||
				r.y + r.h < y);
		}
	};

	Level() : camera(800, 600, 0.2f) {}
	Level(int widthInTiles, int heightInTiles);
	~Level();
	
	inline void SetTile(int x, int y, Tile&& tile);
	inline void SetTile(int x, int y, const Tile& tile);
	inline Tile& GetTile(int x, int y);

	void LoadLevel();

	inline Camera& GetCamera() {return camera;}
	inline int GetWidth() const {return w;}
	inline int GetHeight() const {return h;}
	
	static Level CreateLevelWithRooms(LEVEL_SIZE levelSize, Room::ROOM_SIZE maxRoomSize);
	static Level CreateLevelWithPerlinNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithCA(int levelWidth, int levelHeight);

	void Draw(sf::RenderWindow* window);
private:	
	static Level::Room CreateRoom(std::vector<Room>& rooms, Room::ROOM_TYPE type, Room::ROOM_SIZE size, int levelWidth, int levelHeight);
};

#endif