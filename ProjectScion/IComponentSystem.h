#ifndef _COMPONENT_SYSTEM_I_H
#define _COMPONENT_SYSTEM_I_H

#include "IComponent.h"
#include "Entity.h"

// Mohammed Hossain
// IComponentSystem is an interface that all future component systems will inherit from. CSs should override these pure virtual methods
// and _should_ have some collection of components that it will handle inside the class.

class IComponentSystem
{
public:
    virtual ~IComponentSystem() {}

    inline virtual bool IsRenderingSystem() const { return false; }
    virtual void Update(float dt) = 0;
    
    virtual void RegisterEntity(Entity* ent) = 0;
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr) = 0;
};

#endif