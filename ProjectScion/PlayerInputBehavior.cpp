#include "PlayerInputBehavior.h"
#include "ScionEngine.h"

#include <SFML\Window.hpp>

using namespace sf;

PlayerInputBehavior::PlayerInputBehavior(TransformAttribute* transform, SpriteAttribute* sprite)
    : _transform(transform), _sprite(sprite)
{}

void PlayerInputBehavior::Process()
{
    //auto evts = ScionEngine::GetEvents();

    //for_each(begin(evts), end(evts), [&](sf::Event& evt)
    //{
    //    switch(evt.type)
    //    {
    //    case sf::Event::EventType::KeyPressed:
    //        evt.key.code                        
    //    }
    //})

    bool dirSet = false;
    
    if(Keyboard::isKeyPressed(Keyboard::Left))
    {
        _transform->Move(-1, 0);

        if(!dirSet)
        {
            dirSet = true;
            _transform->Direction = LEFT;

            //change the sprite's texRect or the transform's angle
            _sprite->GetSprite().setTextureRect(sf::IntRect(0, 32, 32, 32));
        }
    }
    if(Keyboard::isKeyPressed(Keyboard::Up))
    {
        _transform->Move(0, -1);

        if(!dirSet)
        {
            dirSet = true;
            _transform->Direction = UP;
            _sprite->GetSprite().setTextureRect(sf::IntRect(32, 32, 32, 32));
        }
    }
    if(Keyboard::isKeyPressed(Keyboard::Right))
    {
        _transform->Move(1, 0);

        if(!dirSet)
        {
            dirSet = true;
            _transform->Direction = RIGHT;
            _sprite->GetSprite().setTextureRect(sf::IntRect(0, 0, 32, 32));
        }
    }
    if(Keyboard::isKeyPressed(Keyboard::Down))
    {
        _transform->Move(0, 1);
        
        if(!dirSet)
        {
            dirSet = true;
            _transform->Direction = DOWN;
            _sprite->GetSprite().setTextureRect(sf::IntRect(32, 0, 32, 32));
        }
    }
}