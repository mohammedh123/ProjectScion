#ifndef _GRAPHICS_COMPONENT_H
#define _GRAPHICS_COMPONENT_H

#include <SFML\Graphics.hpp>
#include "Component.h"

struct GraphicsComponent : public Component
{
	virtual void Draw(sf::RenderWindow*) = 0;
};

#endif