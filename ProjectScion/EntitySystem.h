#ifndef _ENTITY_SYSTEM_H
#define _ENTITY_SYSTEM_H

#include "IComponentSystem.h"

#include <vector>

class EntitySystem
{
    std::vector<std::unique_ptr<IComponentSystem>> registeredSystems;
public:
    void RegisterSystem(IComponentSystem* system);

    void Update(float dt);
    void CreateEntity();
};

#endif