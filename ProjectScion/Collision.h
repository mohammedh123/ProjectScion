#ifndef COLLISION_H
#define COLLISION_H
#include "SFML\Graphics.hpp"
#include <algorithm>
#include <math.h>

class Collision
{
public:
    static bool CircleCollision(const sf::Sprite& Object1, const sf::Sprite& Object2);
    static bool CircleCollision(float radius1, sf::Vector2f position1, float radius2, sf::Vector2f position2);
    static bool BoundingRectCollision(const sf::Sprite& Object1, const sf::Sprite& Object2);
    static bool PixelPerfectCollision(const sf::Sprite& Object1, const sf::Sprite& Object2);
};

#endif