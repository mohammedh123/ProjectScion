#include "AttackSystem.h"

#include <memory>
#include <exception>
#include <string>

using namespace std;

void AttackSystem::Update(float)
{
    CAttack* attack = nullptr;
    Entity* entity = nullptr;

    for(auto itr = cAttacks.begin(); itr != cAttacks.end(); itr++)
    {
        attack = (*itr).second.get();
        entity = (*itr).first;
        
        if(attack->enabled)
        {
            if(attack->SecondsIn() >= attack->duration)
            {
                attack->enabled = false;
            }
            else
            {
                //TODO: handle dmgRect colliding with enemies
                auto resultantRect = attack->dmgRect;
                sf::Transform trans;
            }
        }
    }
}

void AttackSystem::Draw()
{
    CSprite* sprite = nullptr;
    Entity* entity = nullptr;
    for(auto itr = cAttackSprites.begin(); itr != cAttackSprites.end(); itr++)
    {
        sprite = (*itr).second.get();
        entity = (*itr).first;
        
        sprite->GetSprite().setPosition(entity->x, entity->y);

        _window->draw(sprite->GetSprite());
    }
}

void AttackSystem::RegisterEntity(Entity* ent)
{
    throw exception("AttackSystem::RegisterEntity cannot be called with a default component.");
}

void AttackSystem::RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr)
{
    if(componentFlag == ATTACK)
        cAttacks[ent] = unique_ptr<CAttack>(static_cast<CAttack*>(componentPtr));
    else if(componentFlag == SPRITE)
        cAttackSprites[ent] = unique_ptr<CSprite>(static_cast<CSprite*>(componentPtr));
}