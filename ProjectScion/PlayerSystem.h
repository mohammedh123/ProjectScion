#ifndef _PLAYER_SYSTEM_H
#define _PLAYER_SYSTEM_H

#include <ac/es.h>

class PlayerSystem : public ac::es::EntityProcessingSystem
{
    AC_ES_ENTITY_SYSTEM(PlayerSystem)

    float* _dtProvider;
public:
    PlayerSystem(float* dtProvider);
    
    virtual void process(ac::es::EntityPtr e);
};

#endif