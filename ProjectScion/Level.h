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
	struct Room
	{
		enum ROOM_SIZE
		{
			//value is max-size
			SMALL = 5,
			NORMAL = 10,
			LARGE = 20
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
	};

	Camera camera;

	std::vector<std::vector<Tile>> map;
	std::vector<Entity*> objects;

	int w;
	int h;

	void SetDimensions(int w, int h);
	static Level::Room CreateRoom(std::vector<Room>& rooms, Room::ROOM_TYPE type, Room::ROOM_SIZE size, int levelWidth, int levelHeight);
public:

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
	
	static Level CreateLevelWithRooms(int levelWidth, int levelHeight);
	static Level CreateLevelWithPerlinNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithVoronoiNoise(int levelWidth, int levelHeight);
	static Level CreateLevelWithCA(int levelWidth, int levelHeight);

	void Draw(sf::RenderWindow* window);
};

#endif