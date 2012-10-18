#ifndef _SPRITE_ATTR_H
#define _SPRITE_ATTR_H

#include "Attribute.h"

#include <SFML\Graphics.hpp>

class SpriteAttribute : public Attribute
{
    sf::Sprite _sprite;
public:
    SpriteAttribute(int x, int y, sf::Texture& texture);
    SpriteAttribute(int x, int y, sf::IntRect rect, sf::Texture& texture);
    
    inline void UpdateSprite(sf::Vector2f position, float rotation, sf::Vector2f scale)
    {
        _sprite.setPosition(position);
        _sprite.setRotation(rotation);
        _sprite.setScale(scale);
    }

    inline sf::Sprite& GetSprite() { return _sprite; }
};

#endif