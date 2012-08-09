#include "SpriteComponent.h"
#include "TransformComponent.h"
#include <SFML\Graphics.hpp>

SpriteComponent::SpriteComponent(sf::Texture& texture, TransformComponent* transformPtr) : sprite(texture), transform(transformPtr)
{
	if(!transformPtr)
		throw NullTransformComponentException();

	transform = transformPtr;
} 

SpriteComponent::SpriteComponent(sf::Texture& texture, sf::IntRect& rectangle, TransformComponent* transformPtr) : sprite(texture, rectangle), transform(transformPtr)
{
	//these are the exame same fuck c++
	if(!transformPtr)
		throw NullTransformComponentException();

	transform = transformPtr;
}

void SpriteComponent::Draw(sf::RenderWindow* window)
{
	sprite.setPosition(transform->position);
	sprite.setRotation(transform->angle + transform->spriteAngle);
	window->draw(sprite);
}