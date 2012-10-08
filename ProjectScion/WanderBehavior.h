#ifndef WANDER_BEHAVIOR_H
#define WANDER_BEHAVIOR_H

#include "Behavior.h"
#include "TransformAttribute.h"
#include "Level.h"
#include "ScionEngine.h"
#include <math.h>

#include <SFML\Window.hpp>

class WanderBehavior : public Behavior
{
	TransformAttribute* _transform;
	Level& _level;
	float dx, dy;
	sf::FloatRect _rect;
	float _center_x, _center_y;
	float xDist, yDist;
public:
	WanderBehavior(float x, float y, float width, float height, TransformAttribute* transform, Level& level);
	void pickARandomDirection(array<array<Tile*, 3>,3>& neighbors);
	void Process();
};

#endif