#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>
#include <map>
#include <string>
#include <math.h>


class Tile
{
	friend bool operator==(const Tile& lhs, const Tile& rhs);
public:
	enum TYPE
	{
		UNUSED,
		BLANK,
		GROUND,
		WALL,
		CORRIDOR
	};

	Tile::TYPE type;
	bool entrance;

	sf::Color color;
	int x, y;

	//pathfinding
	float G, H;
	inline float GetF() const { return G + H; }
	inline float ManhattanDistance(Tile* tile) const
	{
		return std::abs(tile->x - x) + std::abs(tile->y - y);		
	}
	Tile* parent;
	//end pathfinding

    static const std::map<Tile::TYPE,Tile>& DefaultTiles();

	sf::Sprite baseSprite;
	bool solid;
	static const int SIZE = 32;
	
	Tile() : x(0), y(0), solid(false), type(UNUSED), entrance(false) {}
	Tile(int x, int y) : x(x), y(y), solid(false), type(UNUSED), entrance(false) {}
	explicit Tile(int x, int y, Tile::TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);
	explicit Tile(int x, int y, sf::Texture* image, sf::IntRect rect, Tile::TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);

	~Tile();

	void Draw(sf::RenderWindow* rw);
	void Draw(sf::RenderTexture* rt);
};

bool operator==(const Tile& lhs, const Tile& rhs);
bool operator<(const Tile& lhs, const Tile& rhs);
bool operator!=(const Tile& lhs, const Tile& rhs);

#endif