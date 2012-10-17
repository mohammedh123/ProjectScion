#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML\Graphics.hpp>

#include "Attribute.h"
#include "Behavior.h"

#include <vector>

class Entity
{
	friend class ScionEngine;

	ScionEngine* game;
	Entity(){}
	
	std::vector<Attribute*> attributes;
	std::vector<Behavior*> logicalBehaviors, renderBehaviors;
public:
	void AddBehavior(Behavior* b) { if(b->IsRenderingBehavior()) renderBehaviors.push_back(b); else logicalBehaviors.push_back(b); }

	std::vector<Attribute*> GetAttributes(
};

#endif