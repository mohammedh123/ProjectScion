#ifndef _C_PLAYER_LOGIC_H
#define _C_PLAYER_LOGIC_H

#include "CLogic.h"

class CPlayerLogic : public CLogic
{
public:
    float deltaX, deltaY;

    void Process(float delta);
};

#endif