#include "LogicSystem.h"
#include <exception>

using namespace std;

void LogicSystem::Update(float dt)
{    
    CLogic* logic = nullptr;
    Entity* entity = nullptr;

    for(auto itr = cLogics.begin(); itr != cLogics.end(); itr++)
    {
        logic = (*itr).second.get();
        entity = (*itr).first;
        
        logic->Process(dt, entity);
    }
}

void LogicSystem::RegisterEntity(Entity* ent)
{
    throw exception("LogicSystem::RegisterEntity has been called without a component.");
}

void LogicSystem::RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr)
{    
    if(componentFlag == LOGIC)
        cLogics.insert(make_pair(ent, unique_ptr<CLogic>(static_cast<CLogic*>(componentPtr))));
}