#include "LogicSystem.h"
#include <exception>

using namespace std;

void LogicSystem::Update(float dt)
{
}

void LogicSystem::RegisterEntity(Entity* ent)
{
    throw exception("LogicSystem::RegisterEntity has been called without a component.");
}

void LogicSystem::RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr)
{
}