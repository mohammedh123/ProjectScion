#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H

#include "SFML\System.hpp"
#include "Component.h"

struct TransformComponent : public Component
{
	sf::Vector2f position, velocity, scale;
	float angle, spriteAngle;
};

#endif