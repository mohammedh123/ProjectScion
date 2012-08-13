#include "Tile.h"
#include <SFML\Graphics.hpp>

Tile::Tile(sf::Texture* image) : baseSprite(*image)
{
}

Tile::~Tile()
{

}

void Tile::Draw(int x, int y, sf::RenderTexture* rt)
{
	baseSprite.setPosition((float)x, (float)y);
	rt->draw(baseSprite);
}


void Tile::Draw(int x, int y, sf::RenderWindow* rw)
{
	baseSprite.setPosition((float)x, (float)y);
	rw->draw(baseSprite);
}

