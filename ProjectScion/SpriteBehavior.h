#ifndef _SPRITE_BEHAVIOR_H
#define _SPRITE_BEHAVIOR_H

#include <SFML\Graphics.hpp>

#include "Behavior.h"
#include "TransformAttribute.h"

class SpriteBehavior : public Behavior
{
	TransformAttribute* _transform;
	sf::RenderWindow* _window;
	sf::Sprite _sprite;
public:
	SpriteBehavior(sf::Texture& tex, float x, float y, TransformAttribute* transform, sf::RenderWindow* window);
	SpriteBehavior(sf::Texture& tex, sf::IntRect rect, float x, float y, TransformAttribute* transform, sf::RenderWindow* window);

	void Process();
	bool IsRenderingBehavior() const { return true; }
};

#endif