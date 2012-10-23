#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

class GraphicsSystem : public IComponentSystem
{
public:
    virtual void Update(float dt);
};

#endif