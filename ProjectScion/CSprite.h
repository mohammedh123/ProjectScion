#ifndef _C_SPRITE_H
#define _C_SPRITE_H

#include <SFML\Graphics.hpp>

#include "IComponent.h"

using namespace sf;

class CSprite : public IComponent
{
    sf::Sprite sprite;
public:
    CSprite(sf::Texture& tex, float ox, float oy);
    CSprite(sf::Texture& tex, sf::IntRect rect, float ox, float oy);

    inline sf::Sprite& GetSprite() { return sprite; }
};

#endif