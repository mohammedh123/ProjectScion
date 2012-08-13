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

class ScionEngine
{
private:
	Level currentLevel;
	std::unique_ptr<sf::RenderWindow> window;

	std::unique_ptr<StateManager> stateManager;
	std::unique_ptr<TextureManager> texManager;
	std::unique_ptr<SoundBufferManager> soundBufferManager;
	std::unique_ptr<FontManager> fontManager;
	std::unique_ptr<MusicManager> musicManager;
	std::unique_ptr<ShaderManager> shaderManager;

	std::map<std::string, sf::Font*> fonts;
	std::unique_ptr<sf::Clock> clock;

	std::vector<std::unique_ptr<Entity>> entitys;
	
	std::vector<std::unique_ptr<Behavior>> behaviors;
	std::vector<std::unique_ptr<Attribute>> attributes;
	
	static std::vector<sf::Event> events;
	
	void Init();
	void GameLoop();
	void RenderFrame();
	void ProcessInput();
	void LoadImages();
	void Update();
public:
	ScionEngine();
	~ScionEngine();
	
	sf::Texture* GetTexture(const std::string& name) const { return texManager->GetImage(name);}
	sf::SoundBuffer* GetSoundBuffer(const std::string& name) const { return soundBufferManager->LoadFromFile(name);}
	sf::Font* GetFont(const std::string& name) const { return fontManager->LoadFromFile(name);}
	sf::Music* GetMusic(const std::string& name) const { return musicManager->LoadFromFile(name);}
	sf::Shader* GetShader(const std::string& vShaderName, const std::string& fShaderName) const { return shaderManager->LoadFromFile(vShaderName, fShaderName);}
	sf::Shader* GetShader(const std::string& name, sf::Shader::Type type) const { return shaderManager->LoadFromFile(name, type);}

	inline Level& GetCurrentLevel() { return currentLevel;}
	std::vector<std::unique_ptr<Behavior>>& GetBehaviors() { return behaviors;}
	static const std::vector<sf::Event>& GetEvents() { return ScionEngine::events;}

	void Go();
	Entity* CreateEntity();	
	Behavior* CreateBehavior(Behavior* b);
	Attribute* CreateAttribute(Attribute* a);
};

#endif