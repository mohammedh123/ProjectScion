#ifndef _C_ATTACK_H
#define _C_ATTACK_H

#include <SFML\Graphics.hpp>

#include "IComponent.h"

using namespace sf;

class CAttack : public IComponent
{
public:
    bool enabled;
    float duration, cooldown;
    sf::IntRect dmgRect; //in relation to entity facing right
    sf::Clock attackTimer;
    
    CAttack(sf::IntRect dRect, float duration, float cooldown);

    void Begin();
    float SecondsIn() const { return attackTimer.getElapsedTime().asSeconds(); } //inline
};

#endif