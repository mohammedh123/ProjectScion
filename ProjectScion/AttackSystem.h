#ifndef _ATTACK_SYSTEM_H
#define _ATTACK_SYSTEM_H

#include "IComponentSystem.h"
#include "CAttack.h"
#include "CSprite.h"

#include <SFML\Graphics.hpp>

class AttackSystem : public IComponentSystem
{
    std::map<Entity*, std::unique_ptr<CAttack>> cAttacks;
    std::map<Entity*, std::unique_ptr<CSprite>> cAttackSprites;

    sf::RenderWindow* _window;
public:
    enum
    {
        ATTACK,
        SPRITE
    };

    AttackSystem(sf::RenderWindow* wind) : _window(wind) {}
    
    virtual void Update(float dt);
    virtual void Draw();
    virtual void RegisterEntity(Entity* ent);
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr);
};

#endif