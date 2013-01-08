#ifndef _COLLISION_SYSTEM_H
#define _COLLISION_SYSTEM_H

class CollisionSystem
{
public:
    enum
    {
        COLLISION
    };

    virtual void Update(float dt);
};

#endif