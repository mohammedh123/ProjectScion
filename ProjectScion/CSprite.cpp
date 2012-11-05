#include "CSprite.h"

CSprite::CSprite(sf::Texture& tex, float ox, float oy) : sprite(tex)
{
    sprite.setOrigin(ox,oy);
}

CSprite::CSprite(sf::Texture& tex, sf::IntRect rect, float ox, float oy) : sprite(tex, rect)
{
    sprite.setOrigin(ox,oy);
}