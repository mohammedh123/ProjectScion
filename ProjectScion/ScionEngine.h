#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

//System
#include <memory>

//SFML
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

//Managers
#include "StateManager.h"
#include "ImageManager.h"
#include "FontManager.h"
#include "SoundBufferManager.h"
#include "MusicManager.h"
#include "ShaderManager.h"

//States
#include "State.h"
#include "GameState.h"
#include "SplashScreenState.h"

//Component Systems
#include "SystemManager.h"

class ScionEngine
{
private:
	std::unique_ptr<sf::RenderWindow> window;

	std::unique_ptr<StateManager> stateManager;
	std::unique_ptr<ImageManager> imgManager;
	std::unique_ptr<SoundBufferManager> soundBufferManager;
	std::unique_ptr<FontManager> fontManager;
	std::unique_ptr<MusicManager> musicManager;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<SystemManager> systemManager;

	std::map<std::string, sf::Font*> fonts;
	std::unique_ptr<sf::Clock> clock;

	std::vector<std::unique_ptr<GameObject>> gameObjects;
	
	void Init();
	void GameLoop();
	void RenderFrame();
	void ProcessInput();
	void LoadImages();
	void Update();

	//put systems here that you will use	
public:
	ScionEngine();
	~ScionEngine();

	void Go();
	GameObject* CreateGameObject();
};

#endif