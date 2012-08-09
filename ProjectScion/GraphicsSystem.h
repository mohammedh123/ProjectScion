#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include "ComponentSystem.h"
#include "GraphicsComponent.h"
#include <vector>

class GraphicsSystem : public ComponentSystem
{	
	std::vector<GraphicsComponent*> components;
public:
	void Process(sf::RenderWindow* window);
};

#endif