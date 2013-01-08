#include "GraphicsSystem.h"

#include "CSprite.h"
#include "CPosition.h"

#include <SFML\Graphics.hpp>

using namespace std;
using namespace ac::es;

GraphicsSystem::GraphicsSystem()    
    :   EntityProcessingSystem(ComponentFilter::Requires<CSprite>().Requires<CPosition>())
{
}

void GraphicsSystem::process(EntityPtr e)
{
    auto sprite     = e->getComponent<CSprite>();
    auto position   = e->getComponent<CPosition>();

    sprite->GetCurrentAnimation().setPosition(position->x, position->y);
    sprite->windowToDrawIn->draw(sprite->GetCurrentAnimation());
    //for(auto itr = cSprites.begin(); itr != cSprites.end(); itr++)
    //{
    //    sprite = (*itr).second.get();
    //    entity = (*itr).first;
    //    
    //    sprite->GetCurrentAnimation().setPosition(entity->x, entity->y);

    //    _window->draw(sprite->GetCurrentAnimation());
    //}
}