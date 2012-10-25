#ifndef _COMPONENT_SYSTEM_I_H
#define _COMPONENT_SYSTEM_I_H

#include "IComponent.h"

class IComponentSystem
{
public:
    virtual ~IComponentSystem() {}

    virtual void Update(float dt) = 0;
    
    virtual void RegisterEntity(Entity* ent) = 0;
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr) = 0;
};

#endif