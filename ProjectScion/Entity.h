#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML\Graphics.hpp>

#include <memory>
#include <vector>
#include <array>

#include "IComponent.h"

class EntitySystem;

class Entity
{
    friend class ScionEngine;
	friend class EntitySystem;
    
    ScionEngine* game;
    
    std::vector<std::unique_ptr<IComponent>> components;
public:
    float x, y;
	int id;
};

#endif