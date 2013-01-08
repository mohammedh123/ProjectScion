#include "AttackSystem.h"

#include <memory>
#include <exception>
#include <string>

using namespace std;

void AttackSystem::Update(float)
{
    //CAttack* attack = nullptr;
    //Entity* entity = nullptr;

    //for(auto itr = cAttacks.begin(); itr != cAttacks.end(); itr++)
    //{
    //    attack = (*itr).second.get();
    //    entity = (*itr).first;
    //    
    //    if(attack->enabled)
    //    {
    //        if(attack->SecondsIn() >= attack->duration)
    //        {
    //            attack->enabled = false;
    //        }
    //        else
    //        {
    //            //TODO: handle dmgRect colliding with enemies
    //            auto resultantRect = attack->dmgRect;
    //            sf::Transform trans;
    //        }
    //    }
    //}
}

void AttackSystem::Draw()
{
    //CSprite* sprite = nullptr;
    //Entity* entity = nullptr;
    //for(auto itr = cAttackSprites.begin(); itr != cAttackSprites.end(); itr++)
    //{
    //    sprite = (*itr).second.get();
    //    entity = (*itr).first;
    //    
    //    //sprite->GetCurrentAnimation().setPosition(entity->x, entity->y);

    //    _window->draw(sprite->GetCurrentAnimation());
    //}
}