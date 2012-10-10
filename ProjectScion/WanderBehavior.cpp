#include "WanderBehavior.h"

WanderBehavior::WanderBehavior(float x, float y, float width, float height, TransformAttribute* transform, Level& level)
	: _transform(transform), _level(level)
{
	_rect.width = width;
	_rect.height = height;
	_center_x = x;
	_center_y = y;
	dx = 0;
	dy = 0;
	xDist = 0; 
	yDist = 0;
	srand ( time(NULL) );
}

void WanderBehavior::Process()
{
	_rect.left = _transform->GetPosition().x - _center_x;
	_rect.top = _transform->GetPosition().y - _center_y;

	//Get the current cell of the unit, check collision with surrounding cell
	Tile* tile = &_level.GetTile((int)(_transform->GetPosition().x/32),(int)(_transform->GetPosition().y/32));
	
	_transform->Move(( fabs(dx) <= fabs(xDist))?dx:xDist, (fabs(dy) <= fabs(yDist))?dy:yDist);
	
	(dx <= xDist)?(xDist -= fabs(dx)):(xDist -= xDist);
	(dy <= yDist)?(yDist -= fabs(dy)):(yDist -= yDist);

	
	
	if(xDist == 0 && yDist == 0)
	{
		array<array<Tile*, 3>,3> neighbors;
	_level.GetAllNeighbors(tile, neighbors);
		pickARandomDirection(neighbors);
	}
}

void WanderBehavior::pickARandomDirection(array<array<Tile*, 3>,3>& neighbors)
{
	std::vector<std::pair<int, int>> possibleDirection;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j< 3; j++)
		{
			if(neighbors[i][j]->solid || (i == 1 && j == 1))
				continue;
			possibleDirection.push_back(pair<int, int>(i, j));
		}
	}
	int randomInt = std::rand() % possibleDirection.size();
	dx = possibleDirection[randomInt].first - 1;
	dy = possibleDirection[randomInt].second - 1;
	xDist = fabs(32 * dx);
	yDist = fabs(32 * dy);
}