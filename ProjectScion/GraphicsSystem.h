#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include "IComponentSystem.h"
#include "CSprite.h"

#include <SFML\Graphics.hpp>

class GraphicsSystem : public IComponentSystem
{
    sf::RenderWindow* _window;

    std::map<Entity*, std::unique_ptr<CSprite>> cSprites;
public:
    enum
    {
        SPRITE
    };

    GraphicsSystem(sf::RenderWindow* wind) : _window(wind) {}
    
    inline virtual bool IsRenderingSystem() const { return true; }

    virtual void Update(float dt);
    virtual void RegisterEntity(Entity* ent);
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr);
};

#endif