#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

#include <SFML\Graphics.hpp>
#include "ImageManager.h"
#include "Tile.h"
#include "Camera.h"
#include "Level.h"
#include <memory>

class ScionEngine
{
private:
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<ImageManager> imgManager;
	std::unique_ptr<Camera> camera;

	Level* currentLevel;

	void Init();
	void GameLoop();
	void RenderFrame();
	void ProcessInput();
	void LoadImages();
	void Update();
public:
	ScionEngine();
	~ScionEngine();

	void Go();
};

#endif