#include "SpriteBehavior.h"


SpriteBehavior::SpriteBehavior(sf::Texture& tex, float x, float y, TransformAttribute* transform, sf::RenderTarget* window)
	: _transform(transform), _window(window), _sprite(tex)
{
	_sprite.setOrigin(x, y);
}

SpriteBehavior::SpriteBehavior(sf::Texture& tex, sf::IntRect rect, float x, float y, TransformAttribute* transform, sf::RenderTarget* window)
	: _transform(transform), _window(window), _sprite(tex, rect)
{
	_sprite.setOrigin(x, y);
}

//draw
void SpriteBehavior::Process()
{
	_sprite.setPosition(_transform->GetPosition());
	_sprite.setRotation(_transform->GetAngle());
	_sprite.setScale(_transform->GetScale());

	_window->draw(_sprite);
}