#ifndef _SCION_ENGINE_H
#define _SCION_ENGINE_H

//System
#include <memory>

//SFML
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

//Managers
#include "StateManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SoundBufferManager.h"
#include "MusicManager.h"
#include "ShaderManager.h"

//States
#include "State.h"
#include "GameState.h"
#include "SplashScreenState.h"

#include "Behavior.h"
#include "Attribute.h"

#include <vector>
#include <random>

class ScionEngine
{
private:
	Level currentLevel;
	std::unique_ptr<sf::RenderWindow> window;

	static StateManager* stateManager;
	static TextureManager* texManager;
	static SoundBufferManager* soundBufferManager;
	static FontManager* fontManager;
	static MusicManager* musicManager;
	static ShaderManager* shaderManager;
	static std::mt19937 randEngine;

	std::map<std::string, sf::Font*> fonts;
	std::unique_ptr<sf::Clock> clock;

	std::vector<std::unique_ptr<Entity>> entitys;
	
	std::vector<std::unique_ptr<Behavior>> behaviors;
	std::vector<std::unique_ptr<Attribute>> attributes;
	
	static std::vector<sf::Event> events;
	bool isActive;
	
	void Init();
	void GameLoop();
	void ProcessInput();
	void LoadImages();
	void RenderFrame();
	void Update();  
public:
	ScionEngine();
	~ScionEngine();
	
	static sf::Texture* GetTexture(const std::string& name) { return texManager->GetImage(name);}
	static sf::SoundBuffer* GetSoundBuffer(const std::string& name) { return soundBufferManager->LoadFromFile(name);}
	static sf::Font* GetFont(const std::string& name) { return fontManager->LoadFromFile(name);}
	static sf::Music* GetMusic(const std::string& name) { return musicManager->LoadFromFile(name);}
	static sf::Shader* GetShader(const std::string& vShaderName, const std::string& fShaderName) { return shaderManager->LoadFromFile(vShaderName, fShaderName);}
	static sf::Shader* GetShader(const std::string& name, sf::Shader::Type type) { return shaderManager->LoadFromFile(name, type);}

	inline Level& GetCurrentLevel() { return currentLevel;}
	std::vector<std::unique_ptr<Behavior>>& GetBehaviors() { return behaviors;}
	static const std::vector<sf::Event>& GetEvents() { return ScionEngine::events;}
	static const int GetRandomNumber(int lowerBound=1, int upperBound=100) { return std::uniform_int_distribution<int>(lowerBound, upperBound)(randEngine);}
	inline const bool IsActive() const { return isActive;}

	void Go();
	Entity* CreateEntity();	
	Behavior* CreateBehavior(Behavior* b);
	Attribute* CreateAttribute(Attribute* a);
};

#endif