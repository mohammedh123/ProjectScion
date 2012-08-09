#ifndef _GAMEOBJ_H
#define _GAMEOBJ_H

#include <SFML\Graphics.hpp>
#include "Component.h"

#include <vector>

class GameObject
{
public:
	GameObject() {}

	std::vector<Component*> components;
};

#endif