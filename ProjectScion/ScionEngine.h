#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

#include <SFML\Graphics.hpp>
#include <memory>

class ScionEngine
{
private:
	std::unique_ptr<sf::RenderWindow> window;

	void Init();
	void GameLoop();
	void RenderFrame();
	void ProcessInput();
	void Update();
public:
	ScionEngine();
	~ScionEngine();

	void Go();
};

#endif