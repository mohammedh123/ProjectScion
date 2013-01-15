#ifndef _C_HEALTH_H
#define _C_HEALTH_H

#include <SFML\Graphics.hpp>

#include <ac/es.h>

using namespace sf;

class CHealth : public ac::es::Component<CHealth>
{
public:
    int value;

    CAttack(sf::IntRect dRect, float duration, float cooldown);

    void Begin();
    float SecondsIn() const { return attackTimer.getElapsedTime().asSeconds(); } //inline
};

#endif