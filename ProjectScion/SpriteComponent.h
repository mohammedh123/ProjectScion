#ifndef _SPRITE_COMPONENT_H
#define _SPRITE_COMPONENT_H

#include <SFML\Graphics.hpp>

#include "GraphicsComponent.h"
#include "TransformComponent.h"

struct SpriteComponent : public GraphicsComponent
{
	class NullTransformComponentException {};

	TransformComponent* transform;
	sf::Sprite sprite;
	
	SpriteComponent(sf::Texture& texture, TransformComponent* transformPtr);
	SpriteComponent(sf::Texture& texture, sf::IntRect& rectangle, TransformComponent* transformPtr);

	void Draw(sf::RenderWindow* window);
};

#endif