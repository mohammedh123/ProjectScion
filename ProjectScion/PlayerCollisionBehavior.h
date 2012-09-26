#ifndef PLAYER_COLLISION_BEHAVIOR_H
#define PLAYER_COLLISION_BEHAVIOR_H

#include "TransformAttribute.h"
#include "Level.h"
#include "Collision.h"

class PlayerCollisionBehavior : public Behavior
{
	TransformAttribute* _transform;
	sf::RectangleShape _rect;
	Level& _level;
public:
	PlayerCollisionBehavior(float x, float y, float width, float height, TransformAttribute* transform, Level& level);
	void Process();
};

#endif