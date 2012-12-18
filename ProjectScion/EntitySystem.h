#ifndef _ENTITY_SYSTEM_H
#define _ENTITY_SYSTEM_H

#include "IComponentSystem.h"

#include <vector>
#include <array>
#include <memory>

class Entity;

class EntitySystem
{
    std::vector<std::unique_ptr<IComponentSystem>> registeredSystems;
	static const int MAX_ENTITIES = 40;
	std::array<Entity, MAX_ENTITIES> entities;

	int currentId;

	EntitySystem(const EntitySystem&);
	EntitySystem& operator=(const EntitySystem&);

	inline const int GetNextAvailableId();
public:
	EntitySystem();
    void RegisterSystem(IComponentSystem* system);

    void Update(float dt);
    void Draw();
    Entity* CreateEntity(float x, float y);
};

#endif