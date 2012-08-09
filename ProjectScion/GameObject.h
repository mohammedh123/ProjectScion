#ifndef _GAMEOBJ_H
#define _GAMEOBJ_H

#include <SFML\Graphics.hpp>
#include "Component.h"

#include <list>

class GameObject
{
public:
	GameObject() {}

	std::list<Component*> components;
};

#endif