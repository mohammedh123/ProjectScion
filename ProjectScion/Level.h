#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include <unordered_map>
#include <deque>
#include <set>
#include <array>

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
	enum SIZE
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

	Level() : camera(800, 600, 0.2f) {}
	Level(int widthInTiles, int heightInTiles);
	~Level();
	
	inline void SetTile(int x, int y, Tile& tile)
	{
		map[y][x] = tile;
	}

	inline void SetTile(int x, int y, Tile::TYPE type, int entrance = -1, sf::Vector2i outwards = sf::Vector2i())
	{
		auto& tile = Tile::DefaultTiles().at(type);
		map.at(y).at(x).baseSprite = tile.baseSprite;
		map.at(y).at(x).solid = tile.solid;
		map.at(y).at(x).type = type;
		map.at(y).at(x).color = sf::Color::White;
		map.at(y).at(x).entrance = (entrance != -1 ? (bool)entrance : map.at(y).at(x).entrance);
	}
	
	inline Tile& GetTile(int x, int y)
	{
		return map.at(y).at(x);
	}

	inline const Tile* GetTile(int x, int y) const
	{
		return &(map.at(y).at(x));
	}

	inline bool IsWithinBounds(int x, int y) const
	{
		return x >= 0 && x <= w && y >= 0 && y <= h;
	}

	void LoadLevel();

	inline Camera& GetCamera() {return camera;}
	inline int GetWidth() const {return w;}
	inline int GetHeight() const {return h;}
	
	void Draw(sf::RenderWindow* window);
	void Draw(sf::RenderTexture* texture, bool shadowMap = false, sf::RenderStates* states = nullptr);
	void PrintToImage(std::string filename);

	const Tile* GetRandomTileOfType(Tile::TYPE type) const;
	std::deque<Tile*> FindPath(Tile* start, Tile* end);
	std::deque<Tile>  FindPath(const Tile& start, const Tile& end);
	std::deque<Tile*> FindNearestTile(Tile* start, std::set<Tile*>& tilesToIgnore, Tile::TYPE type);

	void GetNeighbors(const Tile& n, std::deque<Tile>& neighbors);
	void GetNeighbors(const Tile* n, std::deque<Tile*>& neighbors);
	void GetAllNeighbors(const Tile* n, std::array<std::array<Tile*, 3>,3>& neighbors);
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> GetPolygonFromSolidTiles();
private:
	static float HeuristicForNode(const Tile& start, const Tile& end);
	float GetDistance(const Tile& s, const Tile& e) const;
	static std::deque<Tile>& Level::ConstructPath(std::unordered_map<Tile, Tile>& q, std::deque<Tile>& path, const Tile& t);
	static std::deque<Tile*>& Level::ConstructPath(std::unordered_map<Tile*, Tile*>& q, std::deque<Tile*>& path, Tile* t);
	std::deque<Tile*> ConstructPath(Tile* end);
};

#endif