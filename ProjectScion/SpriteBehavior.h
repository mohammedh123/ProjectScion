#ifndef _SPRITE_BEHAVIOR_H
#define _SPRITE_BEHAVIOR_H

#include <SFML\Graphics.hpp>

#include "Behavior.h"
#include "TransformAttribute.h"
#include "SpriteAttribute.h"

class SpriteBehavior : public Behavior
{
    TransformAttribute* _transform;
    SpriteAttribute* _sprite;
    sf::RenderWindow* _window;
public:
    SpriteBehavior(TransformAttribute* transform, SpriteAttribute* sprite, sf::RenderWindow* window);

    void Process();
    bool IsRenderingBehavior() const { return true; }
};

#endif