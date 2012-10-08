#include "PlayerCollisionBehavior.h"

#include "ScionEngine.h"

#include <SFML\Window.hpp>

using namespace sf;

PlayerCollisionBehavior::PlayerCollisionBehavior(float x, float y, float width, float height, TransformAttribute* transform, Level& level)
	: _transform(transform), _level(level)
{
	_rect.width = width;
	_rect.height = height;
	_center_x = x;
	_center_y = y;
}

void PlayerCollisionBehavior::Process()
{
	_rect.left = _transform->GetPosition().x - _center_x;
	_rect.top = _transform->GetPosition().y - _center_y;

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

			sf::FloatRect neighborRect = neighbors[i][j]->baseSprite.getGlobalBounds();

			//Check if there is a collision between the player and a neighbor
			if(_rect.intersects(neighborRect))
			{
				//left
				if(i == 0 && j == 1)
				{
					_transform->Move(neighborRect.left + neighborRect.width - _rect.left, 0);
				}
				//top
				if(i == 1 && j == 0)
				{
					_transform->Move(0, (neighborRect.top + neighborRect.height - _rect.top));
				}
				//right
				if(i == 2 && j == 1)
				{
					_transform->Move(neighborRect.left - (_rect.left + _rect.width), 0);
				}
				//bottom
				if(i == 1 && j == 2)
				{
					_transform->Move(0, (neighborRect.top - (_rect.top + _rect.height)));
				}
				
				//topleft
				if(i == 0 && j == 0)
				{
						
				}
				//topRight
				if(i == 2 && j == 0)
				{
					
				}
				//bottmRight
				if(i == 2 && j == 2)
				{
					
				}
				//bottomLeft
				if(i == 0 && j == 2)
				{
					
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