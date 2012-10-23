#include "WanderBehavior.h"

WanderBehavior::WanderBehavior(float x, float y, float width, float height, TransformAttribute* transform, Level& level)
    : _transform(transform), _level(level)
{
    _rect.width = width;
    _rect.height = height;
    _center_x = x;
    _center_y = y;
    dx = -2;
    dy = -2;
    xDist = 0; 
    yDist = 0;
    srand ( (size_t)time(NULL) );
}

void WanderBehavior::Process()
{
    _rect.left = _transform->GetPosition().x - _center_x;
    _rect.top = _transform->GetPosition().y - _center_y;

    //Get the current cell of the unit, check collision with surrounding cell
    Tile* tile = &_level.GetTile((int)(_transform->GetPosition().x/32),(int)(_transform->GetPosition().y/32));
    
    _transform->Move(( fabs(dx) <= fabs(xDist))?dx:xDist, (fabs(dy) <= fabs(yDist))?dy:yDist);
    
    (fabs(dx) <= xDist)?(xDist -= fabs(dx)):(xDist -= xDist);
    (fabs(dy) <= yDist)?(yDist -= fabs(dy)):(yDist -= yDist);

    
    
    if(xDist == 0 && yDist == 0)
    {
        array<array<Tile*, 3>,3> neighbors;
    _level.GetAllNeighbors(tile, neighbors);
        pickARandomDirection(neighbors);
    }
}

void WanderBehavior::pickARandomDirection(array<array<Tile*, 3>,3>& neighbors)
{
    bool stayOnCourse = false;
    std::vector<std::pair<int, int>> possibleDirection;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j< 3; j++)
        {
            if(neighbors[i][j]->solid || (i == 1 && j == 1))
                continue;
            if(i == dx + 1 && j == dy + 1)
            {
                int chanceToStayOnCourse = std::rand() % 4;
                if(chanceToStayOnCourse <= 2)
                    stayOnCourse = true;
            }
            possibleDirection.push_back(pair<int, int>(i, j));
        }
    }
    if(!stayOnCourse)
    {
        int randomInt = std::rand() % possibleDirection.size();
        dx = (float)possibleDirection[randomInt].first - 1.0f;
        dy = (float)possibleDirection[randomInt].second - 1.0f;
    }
    xDist = fabs(32 * dx);
    yDist = fabs(32 * dy);
}