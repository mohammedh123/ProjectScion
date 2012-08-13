#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>

class Tile
{
private:
public:
	sf::Sprite baseSprite;
	static const int SIZE = 32;

	Tile(){}
	explicit Tile(sf::Texture* image);
	~Tile();

	void Draw(int x, int y, sf::RenderWindow* rw);
	void Draw(int x, int y, sf::RenderTexture* rt);
};

#endif