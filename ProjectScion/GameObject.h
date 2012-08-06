#ifndef _GAMEOBJ_H
#define _GAMEOBJ_H

#include <SFML\Graphics.hpp>

class GameObject
{
protected:
	sf::Vector3f position;
	sf::Sprite sprite;
public:
	GameObject(float x, float y, float z, sf::Texture& tex);
	GameObject(float x, float y, float z, sf::Texture& tex, sf::IntRect& rect);

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* window);
};

#endif