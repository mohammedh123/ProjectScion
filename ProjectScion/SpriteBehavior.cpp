#include "SpriteBehavior.h"


SpriteBehavior::SpriteBehavior(TransformAttribute* transform, SpriteAttribute* sprite, sf::RenderWindow* window)
    : _transform(transform), _sprite(sprite), _window(window)
{
}

//draw
void SpriteBehavior::Process()
{
    _sprite->UpdateSprite(_transform->GetPosition(), _transform->GetAngle(), _transform->GetScale());

    _window->draw(_sprite->GetSprite());
}