#include "LogicSystem.h"

#include "CLogic.h"

using namespace std;
using namespace ac::es;

LogicSystem::LogicSystem(float* dtProvider)    
    :   EntityProcessingSystem(ComponentFilter::Requires<CLogic>()), _dtProvider(dtProvider)
{
}

void LogicSystem::process(ac::es::EntityPtr e)
{   
    auto logic     = e->getComponent<CLogic>();

    logic->logicFunc(*_dtProvider, e); //that should be dt
}