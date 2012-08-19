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
	WALL
};

class Tile
{
public:
	TILE_TYPE type;
	sf::Color color;

    static const std::map<TILE_TYPE,Tile>& DefaultTiles();

	sf::Sprite baseSprite;
	bool solid;
	static const int SIZE = 32;

	Tile() : solid(false), type(UNUSED) {}
	explicit Tile(TILE_TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);
	explicit Tile(sf::Texture* image, sf::IntRect rect, TILE_TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);

	~Tile();

	void Draw(int x, int y, sf::RenderWindow* rw);
	void Draw(int x, int y, sf::RenderTexture* rt);
};

#endif