#include "PlayerSystem.h"

#include "CPlayer.h"
#include "CSprite.h"
#include "CPosition.h"
#include "CDirection.h"

#include "InputManager.h"

using namespace std;
using namespace ac::es;

PlayerSystem::PlayerSystem(float* dtProvider)    
    :   Base(ComponentFilter()
    .requires<CPlayer>()
    .requires<CSprite>()
    .requires<CPosition>()
    .requires<CDirection>()), _dtProvider(dtProvider)
{
}

void PlayerSystem::process(ac::es::EntityPtr e)
{   
    auto sprite     = e->getComponent<CSprite>();
    auto position   = e->getComponent<CPosition>();
    auto direction  = e->getComponent<CDirection>();
    auto player     = e->getComponent<CPlayer>();
    
    bool dirSet = false;
    float deltaY = 0, deltaX = 0;                                                                                                                                                                                               
    
    if(InputManager::IsKeyDown(sf::Keyboard::Left))
    {
        deltaX -= 100 * *_dtProvider;   
        direction->value = Direction::LEFT;
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Up))
    {
        deltaY -= 100 * *_dtProvider;
        direction->value = Direction::UP;
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Right))
    {
        deltaX += 100 * *_dtProvider;
        direction->value = Direction::RIGHT;
    }
    if(InputManager::IsKeyDown(sf::Keyboard::Down))
    {
        deltaY += 100 * *_dtProvider;
        direction->value = Direction::DOWN;
    }

    if(deltaX != 0 || deltaY != 0)
    {
        if(direction->value == Direction::LEFT)
            sprite->SetCurrentAnimation("left_moving");
        if(direction->value == Direction::UP)
            sprite->SetCurrentAnimation("up_moving");
        if(direction->value == Direction::RIGHT)
            sprite->SetCurrentAnimation("right_moving");
        if(direction->value == Direction::DOWN)
            sprite->SetCurrentAnimation("down_moving");        
    }
    else
    {
        if(direction->value == Direction::LEFT)
            sprite->SetCurrentAnimation("left");
        if(direction->value == Direction::UP)
            sprite->SetCurrentAnimation("up");
        if(direction->value == Direction::RIGHT)
            sprite->SetCurrentAnimation("right");
        if(direction->value == Direction::DOWN)
            sprite->SetCurrentAnimation("down");
    }

    position->x += deltaX;
    position->y += deltaY;
}