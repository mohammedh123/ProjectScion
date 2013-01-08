#include "PlayerSystem.h"

#include "CPlayer.h"
#include "CSprite.h"
#include "CPosition.h"

#include "InputManager.h"

using namespace std;
using namespace ac::es;

PlayerSystem::PlayerSystem(float* dtProvider)    
    :   EntityProcessingSystem(ComponentFilter::Requires<CPlayer>().Requires<CSprite>().Requires<CPosition>()), _dtProvider(dtProvider)
{
}

void PlayerSystem::process(ac::es::EntityPtr e)
{   
    auto sprite     = e->getComponent<CSprite>();
    auto position   = e->getComponent<CPosition>();
    
    bool dirSet = false;
    float deltaY = 0, deltaX = 0;                                                                                                                                                                                           
    
    if(InputManager::IsKeyDown(sf::Keyboard::Left))
    {
        deltaX -= 100 * *_dtProvider;
        sprite->SetCurrentAnimation("left");
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Up))
    {
        deltaY -= 100 * *_dtProvider;
        sprite->SetCurrentAnimation("up");
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Right))
    {
        deltaX += 100 * *_dtProvider;
        sprite->SetCurrentAnimation("right");
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Down))
    {
        deltaY += 100 * *_dtProvider;
        sprite->SetCurrentAnimation("down");
    }

    position->x += deltaX;
    position->y += deltaY;
}