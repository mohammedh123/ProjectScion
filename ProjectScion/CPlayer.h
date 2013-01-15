#ifndef _C_PLAYER_H
#define _C_PLAYER_H

#include <ac/es.h>

class CPlayer : public ac::es::Component<CPlayer>
{
public:
    float deltaX, deltaY;

    CPlayer();
};

#endif