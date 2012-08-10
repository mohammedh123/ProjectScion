#ifndef _GAMEOBJ_H
#define _GAMEOBJ_H

#include <SFML\Graphics.hpp>
#include "Component.h"

#include <vector>

class GameObject
{
	std::vector<Component*> components;
public:
	GameObject(){}
	~GameObject(){}
};

#endif