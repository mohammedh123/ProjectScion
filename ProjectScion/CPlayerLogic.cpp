#include "CPlayerLogic.h"
#include "Entity.h"

#include "InputManager.h"
#include <SFML\Window.hpp>

using namespace sf;

void CPlayerLogic::Process(float delta, Entity* ent)
{
    bool dirSet = false;
    deltaX = deltaY = 0;
    
    if(InputManager::IsKeyDown(Keyboard::Left))
    {
        deltaX -= 100*delta;
    }
    if(InputManager::IsKeyDown(Keyboard::Up))
    {
        deltaY -= 100*delta;
    }
    if(InputManager::IsKeyDown(Keyboard::Right))
    {
        deltaX += 100*delta;
    }
    if(InputManager::IsKeyDown(Keyboard::Down))
    {
        deltaY += 100*delta;
    }

    ent->Move(deltaX, deltaY);
}