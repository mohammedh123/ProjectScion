#ifndef _COMPONENT_SYSTEM_I_H
#define _COMPONENT_SYSTEM_I_H

class IComponentSystem
{

public:
    virtual ~IComponentSystem() {}

    virtual void Update(float dt) = 0;
};

#endif