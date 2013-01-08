#ifndef _C_ATTACK_H
#define _C_ATTACK_H

#include <SFML\Graphics.hpp>

#include <ac/es.h>

using namespace sf;

class CAttack : public ac::es::Component
{
    AC_ES_COMPONENT(CAttack)
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