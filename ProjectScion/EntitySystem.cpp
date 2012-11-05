#include "EntitySystem.h"
#include "Entity.h"

using namespace std;

EntitySystem::EntitySystem() : currentId(0)
{
	for(int i = 0; i < MAX_ENTITIES; i++)
		entities[i].id = -1;
}

void EntitySystem::RegisterSystem(IComponentSystem* system)
{
    registeredSystems.push_back(unique_ptr<IComponentSystem>(static_cast<IComponentSystem*>(system)));
}

void EntitySystem::Update(float dt)
{
    for(int i = 0; i < registeredSystems.size(); i++)
        if(!registeredSystems[i]->IsRenderingSystem())
            registeredSystems[i]->Update(dt);
}

void EntitySystem::Draw(float dt)
{
    for(int i = 0; i < registeredSystems.size(); i++)
        if(registeredSystems[i]->IsRenderingSystem())
            registeredSystems[i]->Update(dt);
}

Entity* EntitySystem::CreateEntity(float x, float y)
{
	int eId = GetNextAvailableId();

	entities[eId].id = eId;
    entities[eId].x = x;
    entities[eId].y = y;

	return &entities[eId];
}

inline const int EntitySystem::GetNextAvailableId()
{
	int numTries = 0, curId = currentId;

	while(numTries < MAX_ENTITIES && entities[curId].id != -1)
	{
		curId++;
		numTries++;

		curId = currentId >= MAX_ENTITIES ? 0 : currentId;
	}

	currentId = curId;
	return currentId;
}