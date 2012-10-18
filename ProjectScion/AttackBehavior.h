#ifndef _ATTACK_BEHAVIOR_H
#define _ATTACK_BEHAVIOR_H

#include "TransformAttribute.h"
#include "Behavior.h"
#include <SFML\Graphics.hpp>

class AttackBehavior : public Behavior
{
    sf::IntRect _rect;
    float _delay, _cooldown;
    TransformAttribute* _transform;
    std::vector<std::unique_ptr<Entity>>& _entities;
public:
    AttackBehavior(sf::IntRect attackRect, float delay, float cooldown,  std::vector<std::unique_ptr<Entity>>& entities, TransformAttribute* trans);

    void Process();
};

#endif