#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>

class Tile
{
private:
	sf::Sprite baseSprite;
public:
	static const int SIZE = 32;

	Tile(sf::Texture* image);
	~Tile();

	void Draw(int x, int y, sf::RenderWindow* rw);
};

#endif