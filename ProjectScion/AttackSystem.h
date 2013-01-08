#ifndef _ATTACK_SYSTEM_H
#define _ATTACK_SYSTEM_H

#include "CAttack.h"
#include "CSprite.h"

#include <SFML\Graphics.hpp>
#include <ac/es.h>

class AttackSystem
{
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
};

#endif