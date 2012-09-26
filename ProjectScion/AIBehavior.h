#ifndef AI_BEHAVIOR_H
#define AI_BEHAVIOR_H

#include "TransformAttribute.h"
#include "Level.h"

enum AI_STATE
{
	WANDER, APPROACH, ATTACK, DIE, TOTAL_AI_STATE
};

class AIBehavior : public Behavior
{
	AI_STATE _current_state;
public:
	AIBehavior(AI_STATE start_state);
	void Process();
};

#endif