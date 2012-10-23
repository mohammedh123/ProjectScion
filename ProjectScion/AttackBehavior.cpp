#include "AttackBehavior.h"

#include <vector>
#include <memory>

using namespace std;

AttackBehavior::AttackBehavior(sf::IntRect attackRect, float delay, float cooldown, vector<unique_ptr<Entity>>& entities, TransformAttribute* trans)
    : _rect(attackRect), _delay(delay), _cooldown(cooldown), _transform(trans), _entities(entities)
{
}

void AttackBehavior::Process()
{
    sf::IntRect rar(_rect);

    if(_transform->Direction == UP)
    {
        int offsetX = rar.left;
        int offsetY = rar.top;
        int oldW = rar.width;

        rar.width = rar.height;
        rar.height = oldW;

        rar.top = -offsetX - rar.height;
        rar.left = -rar.width + offsetY;
    }
    else if(_transform->Direction == DOWN)
    {
        int offsetX = rar.left;
        int offsetY = rar.top;
        int oldW = rar.width;

        rar.width = rar.height;
        rar.height = oldW;

        rar.top = offsetX;
        rar.left = -rar.width + offsetY;        
    }
    else if(_transform->Direction == RIGHT)
    {
        //leave alone
    }
    else if(_transform->Direction == LEFT)
    {
        rar.left = -rar.left - rar.width;
    }
    
    rar.left += (int)_transform->GetPosition().x;
    rar.top += (int)_transform->GetPosition().y;

    //now check against all enemies hitbox
    for(size_t i = 0; i < _entities.size(); i++)
    {
        Entity* e = _entities[i].get();

        //if(
    }
}