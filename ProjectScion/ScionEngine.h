#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

#include <SFML\Graphics.hpp>
#include <memory>

class Engine
{
private:
	std::unique_ptr<sf::RenderWindow> window;
};

#endif