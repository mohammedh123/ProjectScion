#ifndef _GAMEOBJ_H
#define _GAMEOBJ_H

#include <SFML\Graphics.hpp>
#include "Component.h"

#include <vector>

class GameObject
{
	friend class ScionEngine;

	std::vector<Component*> components;
	GameObject(){}
public:
};

#endif