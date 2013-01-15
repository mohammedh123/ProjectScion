#ifndef _C_MOVEMENT_H
#define _C_MOVEMENT_H

#include <ac/es.h>
#include "DirectionEnum.h"

class CDirection : public ac::es::Component<CDirection>
{
public:
    Direction value, lastFacingDirection;
};

#endif