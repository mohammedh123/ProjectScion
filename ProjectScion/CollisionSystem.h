#ifndef _COLLISION_SYSTEM_H
#define _COLLISION_SYSTEM_H

class CollisionSystem : public IComponentSystem
{
public:
    enum
    {
        COLLISION
    };

    virtual void Update(float dt);
    virtual void RegisterEntity(Entity* ent);
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr);
};

#endif