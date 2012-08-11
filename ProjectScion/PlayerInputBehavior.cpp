#include "PlayerInputBehavior.h"
#include "ScionEngine.h"

#include <SFML\Window.hpp>

using namespace sf;

PlayerInputBehavior::PlayerInputBehavior(TransformAttribute* transform)
	: _transform(transform)
{}

void PlayerInputBehavior::Process()
{
	//auto evts = ScionEngine::GetEvents();

	//for_each(begin(evts), end(evts), [&](sf::Event& evt)
	//{
	//	switch(evt.type)
	//	{
	//	case sf::Event::EventType::KeyPressed:
	//		evt.key.code						
	//	}
	//});
	
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		_transform->Move(-1, 0);
	}
	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		_transform->Move(0, -1);
	}
	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		_transform->Move(1, 0);
	}
	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		_transform->Move(0, 1);
	}
}