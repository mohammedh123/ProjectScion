#include "CPlayerLogic.h"

#include "InputManager.h"
#include <SFML\Window.hpp>

using namespace sf;

void CPlayerLogic::Process(float delta)
{
    bool dirSet = false;
    deltaX = deltaY = 0;
    
    if(InputManager::IsKeyPressed(Keyboard::Left))
    {
        deltaX -= 10*delta;
    }
    if(InputManager::IsKeyPressed(Keyboard::Up))
    {
        deltaY -= 10*delta;
    }
    if(InputManager::IsKeyPressed(Keyboard::Right))
    {
        deltaX += 10*delta;
    }
    if(InputManager::IsKeyPressed(Keyboard::Down))
    {
        deltaY += 10*delta;
    }
}