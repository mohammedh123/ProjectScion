#include "SpriteAttribute.h"

SpriteAttribute::SpriteAttribute(int x, int y, sf::Texture& texture) : _sprite(texture)
{
    _sprite.setOrigin((float)x,(float)y);
}

SpriteAttribute::SpriteAttribute(int x, int y, sf::IntRect rect, sf::Texture& texture) : _sprite(texture, rect)
{
    _sprite.setOrigin((float)x,(float)y);
}