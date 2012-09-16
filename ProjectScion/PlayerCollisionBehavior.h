#ifndef PLAYER_COLLISION_BEHAVIOR_H
#define PLAYER_COLLISION_BEHAVIOR_H

#include "TransformAttribute.h"
#include "Level.h"
#include "Collision.h"

class PlayerCollisionBehavior : public Behavior
{
	TransformAttribute* _transform;
	sf::Sprite _sprite;
	Level& _level;
public:
	PlayerCollisionBehavior(sf::Texture& tex, float x, float y, TransformAttribute* transform, Level& level);
	PlayerCollisionBehavior(sf::Texture& tex, sf::IntRect rect, float x, float y, TransformAttribute* transform, Level& level);
	void Process();
};

#endif