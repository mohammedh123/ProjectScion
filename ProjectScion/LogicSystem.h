#ifndef _LOGIC_SYSTEM_H
#define _LOGIC_SYSTEM_H

#include <ac/es.h>

class LogicSystem : public ac::es::EntityProcessingSystem<LogicSystem>
{
    float* _dtProvider;
public:
    LogicSystem(float* dtProvider);
    
    virtual void process(ac::es::EntityPtr e);
};

#endif