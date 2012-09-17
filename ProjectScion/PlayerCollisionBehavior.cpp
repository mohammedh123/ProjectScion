#include "PlayerCollisionBehavior.h"

#include "ScionEngine.h"

#include <SFML\Window.hpp>

using namespace sf;

PlayerCollisionBehavior::PlayerCollisionBehavior(sf::Texture& tex, float x, float y, TransformAttribute* transform, Level& level)
	: _transform(transform), _level(level),_sprite(tex)
{
	_sprite.setOrigin(x, y);
}

PlayerCollisionBehavior::PlayerCollisionBehavior(sf::Texture& tex, sf::IntRect rect, float x, float y, TransformAttribute* transform, Level& level)
	: _transform(transform), _level(level),_sprite(tex, rect)
{
	_sprite.setOrigin(x, y);
}

void PlayerCollisionBehavior::Process()
{
	_sprite.setPosition(_transform->GetPosition());
	_sprite.setRotation(_transform->GetAngle());
	_sprite.setScale(_transform->GetScale());

	//Get the current cell of the unit, check collision with surrounding cell
	Tile* tile = &_level.GetTile((int)(_transform->GetPosition().x/32),(int)(_transform->GetPosition().y/32));

	//Get the neighbors
	//std::deque<Tile*> neighbors;
	//_level.GetNeighbors(tile, neighbors);
	array<array<Tile*, 3>,3> neighbors;
	_level.GetAllNeighbors(tile, neighbors);
	//neighbors.push_front(tile);
	//Go through the neighbors
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j< 3; j++)
		{
			if(!neighbors[i][j]->solid)
				continue;
			//Update the position of the sprite
			neighbors[i][j]->baseSprite.setPosition((float)neighbors[i][j]->x*neighbors[i][j]->SIZE, (float)neighbors[i][j]->y*neighbors[i][j]->SIZE);

			//Check if there is a collision between the player and a neighbor
			if(Collision::BoundingRectCollision(_sprite, neighbors[i][j]->baseSprite))
			{
				sf::FloatRect neighborRect = neighbors[i][j]->baseSprite.getGlobalBounds();
				sf::FloatRect playerRect = _sprite.getGlobalBounds();

				//left
				if(i == 0 && j == 1)
				{
					_transform->Move(neighborRect.left + neighborRect.width - playerRect.left, 0);
				}
				//top
				else if(i == 1 && j == 0)
				{
					_transform->Move(0, (neighborRect.top + neighborRect.height - playerRect.top));
				}
				//right
				else if(i == 2 && j == 1)
				{
					_transform->Move(neighborRect.left - (playerRect.left + playerRect.width), 0);
				}
				//bottom
				else if(i == 1 && j == 2)
				{
					_transform->Move(0, (neighborRect.top - (playerRect.top + playerRect.height)));
				}
				/*
				if(Keyboard::isKeyPressed(Keyboard::Left))
				{
					_transform->Move(1, 0);
				}
				if(Keyboard::isKeyPressed(Keyboard::Up))
				{
					_transform->Move(0, 1);
				}
				if(Keyboard::isKeyPressed(Keyboard::Right))
				{
					_transform->Move(-1, 0);
				}
				if(Keyboard::isKeyPressed(Keyboard::Down))
				{
					_transform->Move(0, -1);
				}
				*/
				/*
				sf::FloatRect neighborRect = neighbors[i][j]->baseSprite.getGlobalBounds();
				sf::FloatRect playerRect = _sprite.getGlobalBounds();
				if(Keyboard::isKeyPressed(Keyboard::Left))
				{			
					_transform->Move(neighborRect.left + neighborRect.width - playerRect.left, 0);
				}
				if(Keyboard::isKeyPressed(Keyboard::Up))
				{
					_transform->Move(0, (neighborRect.top + neighborRect.height - playerRect.top));
				}
				if(Keyboard::isKeyPressed(Keyboard::Right))
				{
					_transform->Move(neighborRect.left - (playerRect.left + playerRect.width), 0);
				}
				if(Keyboard::isKeyPressed(Keyboard::Down))
				{
					_transform->Move(0, (neighborRect.top - playerRect.top + playerRect.height));
				}
				*/
			}
		}
	}
}