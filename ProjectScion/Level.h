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
	Level() : camera(800, 600, 0.2f) {}
	Level(int widthInTiles, int heightInTiles);
	~Level();

	void AddTile(int x, int y, Tile&& tile);
	Tile& GetTile(int x, int y);

	void LoadLevel();

	inline Camera& GetCamera() {return camera;}
	inline int GetWidth() const {return w;}
	inline int GetHeight() const {return h;}

	static Level CreateLevel(int levelWidth, int levelHeight);

	void Draw(sf::RenderWindow* window);
};

#endif