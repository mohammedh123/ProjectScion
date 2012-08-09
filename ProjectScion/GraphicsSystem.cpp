#include "GraphicsSystem.h"
#include <algorithm>

using namespace std;

void GraphicsSystem::Process(sf::RenderWindow* window)
{
	for_each(begin(components), end(components), [&](GraphicsComponent* c)
	{
		c->Draw(window);
	});
}