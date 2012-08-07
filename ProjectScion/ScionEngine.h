#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

#include <SFML\Graphics.hpp>
#include "ImageManager.h"
#include "FontManager.h"
#include "Tile.h"
#include "Camera.h"
#include "Level.h"
#include <memory>
#include "SplashScreen.h"
#include "SFML/System/Clock.hpp"
#include "SoundBufferManager.h"

enum GameState
{
	GS_SPLASH_SCREEN,
	GS_GAME
};

class ScionEngine
{
private:
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<ImageManager> imgManager;
	std::unique_ptr<SoundBufferManager> soundBufferManager;
	std::unique_ptr<FontManager> fontManager;
	
	std::unique_ptr<Camera> camera;
	std::map<std::string, sf::Font*> fonts;
	std::unique_ptr<SplashScreen> splashScreen;
	std::unique_ptr<sf::Clock> clock;

	Level* currentLevel;
	GameState currentGameState;

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