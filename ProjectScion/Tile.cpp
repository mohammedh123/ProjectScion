#include "Tile.h"
#include <SFML\Graphics.hpp>

Tile::Tile(sf::Texture* image)
{
	baseSprite.setTexture(*image, true);
}

Tile::~Tile()
{

}

void Tile::Draw(int x, int y, sf::RenderWindow* rw)
{
	baseSprite.setPosition((float)x, (float)y);
	rw->draw(baseSprite);
}

