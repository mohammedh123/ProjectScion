#ifndef _LEVEL_PARTITION_H
#define _LEVEL_PARTITION_H

#include "Room.h"

struct LEVEL_PARTITION
{
    ROOM room;
    int id, left, right, parent, x,y,w,h;

    LEVEL_PARTITION() : x(0), w(0), h(0), y(0), id(-1), left(-1), right(-1), parent(-1) {}

    LEVEL_PARTITION(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), id(-1), left(-1), right(-1), parent(-1) {}

    inline bool isValid(int minRoomSize) const    { return w >= minRoomSize && h >= minRoomSize;    }
    inline int area() const { return h*w;}

    void split(bool hSplit, int splitPos, LEVEL_PARTITION& a, LEVEL_PARTITION& b);
    bool testSplit(bool hSplit, int splitPos, int minRoomSize);
    bool testSplitAll(bool hSplit, int minRoomSize);

};
                                                                                                                
bool operator==(const LEVEL_PARTITION& lhs, const LEVEL_PARTITION& rhs);

#endif