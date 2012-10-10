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
public:
	AttackBehavior(sf::IntRect attackRect, float delay, float cooldown, TransformAttribute* trans);

	void Process();
};

#endif