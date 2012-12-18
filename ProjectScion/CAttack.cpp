#include "CAttack.h"

CAttack::CAttack(sf::IntRect dRect, float duration, float cooldown) : enabled(false), dmgRect(dRect), duration(duration), cooldown(cooldown)
{
    attackTimer.restart();
}

void CAttack::Begin()
{
    enabled = true;
    attackTimer.restart();
}