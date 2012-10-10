#include "AttackBehavior.h"

AttackBehavior::AttackBehavior(sf::IntRect attackRect, float delay, float cooldown, TransformAttribute* trans)
	: _rect(attackRect), _delay(delay), _cooldown(cooldown), _transform(trans)
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
}