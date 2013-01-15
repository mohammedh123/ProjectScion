#ifndef _ENTITY_FACTORY_H
#define _ENTITY_FACTORY_H

#include "Components.h"
#include <ac/es.h>

class Entity;

class EntityFactory
{
public:
    static ac::es::Scene* gameScene;
    
    static ac::es::Entity* CreatePlayer(float x, float y, sf::RenderWindow* window);
    static ac::es::Entity* CreateTestEnemy(float x, float y, sf::RenderWindow* window);
};

#endif