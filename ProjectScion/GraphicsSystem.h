#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <ac/es.h>

class GraphicsSystem : public ac::es::EntityProcessingSystem<GraphicsSystem>
{
public:
    GraphicsSystem();
    
    virtual void process(ac::es::EntityPtr e);
};

#endif