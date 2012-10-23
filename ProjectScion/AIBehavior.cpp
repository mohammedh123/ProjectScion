#include "AIBehavior.h"

AIBehavior::AIBehavior(AI_STATE start_state = WANDER):_current_state(start_state)
{
}

void AIBehavior::Process()
{
    switch(_current_state)
    {
    case DIE:
        break;
    default:
        break;
    }
}