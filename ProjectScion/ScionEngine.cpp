#include "ScionEngine.h"

#include "GraphicsSystem.h"
#include "TransformSystem.h"

using namespace std;

ScionEngine::ScionEngine()
{
}

ScionEngine::~ScionEngine()
{
	
}

void ScionEngine::Init()
{
	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Project Scion"));		

	imgManager = unique_ptr<ImageManager>(new ImageManager());
	soundBufferManager = unique_ptr<SoundBufferManager>(new SoundBufferManager());
	fontManager = unique_ptr<FontManager>(new FontManager());
	stateManager = unique_ptr<StateManager>(new StateManager());
	musicManager = unique_ptr<MusicManager>(new MusicManager());
	shaderManager = unique_ptr<ShaderManager>(new ShaderManager());
	systemManager = unique_ptr<SystemManager>(new SystemManager());
	stateManager->LoadResourceManager(imgManager.get(), soundBufferManager.get(), fontManager.get(), musicManager.get(), shaderManager.get());
	
	systemManager->System<GraphicsSystem>("graphics");
	systemManager->System<TransformSystem>("transform");

	systemManager->Initialize();
	
	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
	stateManager->PushState(new GameState());
	//stateManager->PushState(new SplashScreenState(4.0f, imgManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	LoadImages();
	clock = unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();
}

void ScionEngine::RenderFrame()
{
	window->clear();
	stateManager->Draw(window.get());
	window->display();
}

void ScionEngine::ProcessInput()
{
	//This input processor only deals with non keyboard and mouse presses	
	sf::Event evt;
	while(window->pollEvent(evt))
	{
		switch(evt.type)
		{
		case sf::Event::Closed:
				window->close();
				break;
		}
	}
} 

void ScionEngine::LoadImages()
{
	//preload images here
	imgManager->GetImage("tiles.png");
	imgManager->GetImage("player.png");
}

void ScionEngine::Update()
{
	float delta = clock->getElapsedTime().asSeconds();
	clock->restart();
	stateManager->Update(delta, window.get());
}

void ScionEngine::GameLoop()
{
	while(window->isOpen())
	{
		ProcessInput();
		Update();
		RenderFrame();
	}
}

void ScionEngine::Go()
{
	Init();

	GameLoop();
}
	
GameObject* ScionEngine::CreateGameObject()
{
	gameObjects.push_back(unique_ptr<GameObject>(new GameObject()));

	return gameObjects.back().get();
}