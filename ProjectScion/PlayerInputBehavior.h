#ifndef _PLAYER_INPUT_BEHAVIOR_H
#define _PLAYER_INPUT_BEHAVIOR_H

#include "Behavior.h"
#include "TransformAttribute.h"
#include "SpriteAttribute.h"

class PlayerInputBehavior : public Behavior
{
    TransformAttribute* _transform;
    SpriteAttribute* _sprite;
public:
    PlayerInputBehavior(TransformAttribute* transform, SpriteAttribute* sprite);

    void Process();
};

#endif