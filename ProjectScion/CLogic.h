#ifndef _C_LOGIC_H
#define _C_LOGIC_H

#include <ac/es.h>

// Mohammed Hossain
// CLogic is an abstract class that all logic components should update (logic components are components that update the entity's state 
// based on input, where input is either logic or from keyboard/IO devices.

class CLogic : public ac::es::Component
{
    AC_ES_COMPONENT(CLogic)
public:
    typedef void (*ProcessFunc)(float, ac::es::Entity*);

    ProcessFunc logicFunc;
};

#endif