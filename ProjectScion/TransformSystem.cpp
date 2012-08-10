#include "TransformSystem.h"

using namespace std;

void TransformSystem::Process()
{
	for_each(begin(components), end(components), [&](TransformComponent* c)
	{
		c->position.x += c->velocity.x;
		c->position.y += c->velocity.y;
	});
}

void TransformSystem::Initialize()
{
}