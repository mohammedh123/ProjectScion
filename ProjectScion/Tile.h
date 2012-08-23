#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>
#include <map>
#include <string>

enum TILE_TYPE
{
	UNUSED,
	BLANK,
	GROUND,
	WALL,
	CORRIDOR
};

class Tile
{
	friend bool operator==(const Tile& lhs, const Tile& rhs);
public:
	TILE_TYPE type;
	sf::Color color;
	int x, y;

    static const std::map<TILE_TYPE,Tile>& DefaultTiles();

	sf::Sprite baseSprite;
	bool solid;
	static const int SIZE = 32;
	
	Tile() : x(0), y(0), solid(false), type(UNUSED) {}
	Tile(int x, int y) : x(x), y(y), solid(false), type(UNUSED) {}
	explicit Tile(int x, int y, TILE_TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);
	explicit Tile(int x, int y, sf::Texture* image, sf::IntRect rect, TILE_TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);

	~Tile();

	void Draw(sf::RenderWindow* rw);
	void Draw(sf::RenderTexture* rt);
};

bool operator==(const Tile& lhs, const Tile& rhs);
bool operator<(const Tile& lhs, const Tile& rhs);
bool operator!=(const Tile& lhs, const Tile& rhs);

#endif