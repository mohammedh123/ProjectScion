#include "GraphicsSystem.h"

#include "CSprite.h"
#include "CPosition.h"

#include <SFML\Graphics.hpp>

using namespace std;
using namespace ac::es;

GraphicsSystem::GraphicsSystem()    
    :   EntityProcessingSystem(ComponentFilter().requires<CSprite>().requires<CPosition>())
{
}

void GraphicsSystem::process(EntityPtr e)
{
    auto sprite     = e->getComponent<CSprite>();
    auto position   = e->getComponent<CPosition>();

    sprite->GetCurrentAnimation().setPosition(position->x, position->y);
    sprite->windowToDrawIn->draw(sprite->GetCurrentAnimation());
}