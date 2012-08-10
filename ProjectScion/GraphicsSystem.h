#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include "ComponentSystem.h"
#include "GraphicsComponent.h"
#include <vector>
#include <memory>

class GraphicsSystem : public ComponentSystem
{	
	std::vector<std::unique_ptr<GraphicsComponent>> components;
public:
	void Process(sf::RenderWindow* window);
	void Initialize();
};

#endif