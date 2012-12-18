#include "GraphicsSystem.h"

#include <memory>
#include <exception>
#include <string>

using namespace std;

void GraphicsSystem::Draw()
{
    CSprite* sprite = nullptr;
    Entity* entity = nullptr;
    for(auto itr = cSprites.begin(); itr != cSprites.end(); itr++)
    {
        sprite = (*itr).second.get();
        entity = (*itr).first;
        
        sprite->GetSprite().setPosition(entity->x, entity->y);

        _window->draw(sprite->GetSprite());
    }
}

void GraphicsSystem::RegisterEntity(Entity* ent)
{
    throw exception("GraphicsSystem::RegisterEntity cannot be called with a default component.");
}

void GraphicsSystem::RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr)
{
    if(componentFlag == SPRITE)
        cSprites[ent] = unique_ptr<CSprite>(static_cast<CSprite*>(componentPtr));
}