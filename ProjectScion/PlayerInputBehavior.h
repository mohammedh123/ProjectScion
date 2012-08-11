#ifndef _PLAYER_INPUT_BEHAVIOR_H
#define _PLAYER_INPUT_BEHAVIOR_H

#include "Behavior.h"
#include "TransformAttribute.h"

class PlayerInputBehavior : public Behavior
{
	TransformAttribute* _transform;
public:
	PlayerInputBehavior(TransformAttribute* transform);

	void Process();
};

#endif