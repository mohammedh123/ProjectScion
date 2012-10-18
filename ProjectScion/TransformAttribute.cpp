#include "TransformAttribute.h"

TransformAttribute::TransformAttribute(sf::Vector2f& position, sf::Vector2f& velocity, sf::Vector2f& scale, float angle, float spriteAngle)
    : _position(position), _velocity(velocity), _scale(scale), _angle(angle), _spriteAngle(spriteAngle)
{
}

TransformAttribute::TransformAttribute(float x, float y, float vx, float vy, float sx, float sy, float angle, float spriteAngle)
    : _position(x, y), _velocity(vx, vy), _scale(sx, sy), _angle(angle), _spriteAngle(spriteAngle)
{
}

void TransformAttribute::Move(float dx, float dy)
{
    _position.x += dx;
    _position.y += dy;
}

void TransformAttribute::MoveTo(float x, float y)
{
    _position.x = x;
    _position.y = y;
}