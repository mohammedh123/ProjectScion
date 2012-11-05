#ifndef LOGIC_SYSTEM_H
#define LOGIC_SYSTEM_H

#include <map>

#include "IComponentSystem.h"
#include "CLogic.h"

class LogicSystem : public IComponentSystem
{
    std::multimap<Entity*, std::unique_ptr<CLogic>> cLogics;
public:    
    enum
    {
        LOGIC
    };
    
    void Update(float dt);
    void RegisterEntity(Entity* ent);
    void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr);
};

#endif