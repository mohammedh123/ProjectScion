#include "GameObject.h"

GameObject::GameObject(float x, float y, float z, sf::Texture& tex)
{
	position = sf::Vector3f(x,y,z);
	sprite = sf::Sprite(tex);
}

GameObject::GameObject(float x, float y, float z, sf::Texture& tex, sf::IntRect& rect)
{
	position = sf::Vector3f(x,y,z);
	sprite = sf::Sprite(tex, rect);
}

void GameObject::Update(float dt)
{
}

void GameObject::Draw(sf::RenderWindow* window)
{
	window->draw(sprite);
}