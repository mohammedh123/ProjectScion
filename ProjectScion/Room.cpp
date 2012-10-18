#include "Room.h"

bool operator==(const ROOM& lhs, const ROOM& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
}

bool operator!=(const ROOM& lhs, const ROOM& rhs)
{
    return !(lhs == rhs);
}