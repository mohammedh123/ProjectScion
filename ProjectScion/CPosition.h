#ifndef _C_POSITION_H
#define _C_POSITION_H

#include <ac/es.h>

class CPosition : public ac::es::Component<CPosition>
{
public:
    float x, y;

    CPosition(float x, float y) : x(x), y(y) {}
};

#endif