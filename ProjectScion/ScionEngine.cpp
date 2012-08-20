#include "ScionEngine.h"

#include "SpriteBehavior.h"
#include "PlayerInputBehavior.h"

#include <string>
#include <sstream>
#include <windows.h>

using namespace std;

std::vector<sf::Event>					ScionEngine::events(8);
std::unique_ptr<StateManager>			ScionEngine::stateManager(new StateManager());
std::unique_ptr<TextureManager>			ScionEngine::texManager(new TextureManager());
std::unique_ptr<SoundBufferManager>		ScionEngine::soundBufferManager(new SoundBufferManager());
std::unique_ptr<FontManager>			ScionEngine::fontManager(new FontManager());
std::unique_ptr<MusicManager>			ScionEngine::musicManager(new MusicManager());
std::unique_ptr<ShaderManager>			ScionEngine::shaderManager(new ShaderManager());
std::mt19937							ScionEngine::randEngine(GetTickCount());

ScionEngine::ScionEngine()
{
	isActive = true;
}

ScionEngine::~ScionEngine()
{
	
}

void ScionEngine::Init()
{
	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Project Scion"));
	window->setFramerateLimit(60);

	texManager = unique_ptr<TextureManager>(new TextureManager());
	soundBufferManager = unique_ptr<SoundBufferManager>(new SoundBufferManager());
	fontManager = unique_ptr<FontManager>(new FontManager());
	stateManager = unique_ptr<StateManager>(new StateManager());
	musicManager = unique_ptr<MusicManager>(new MusicManager());
	shaderManager = unique_ptr<ShaderManager>(new ShaderManager());
	
	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
	stateManager->PushState(new GameState(), this);
	//stateManager->PushState(new SplashScreenState(4.0f, texManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	LoadImages();
	clock = unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();

	ScionEngine::events.reserve(8);

	auto player = CreateEntity();
	TransformAttribute* trans = static_cast<TransformAttribute*>(CreateAttribute(new TransformAttribute(0, 0, 0, 0)));
	player->AddBehavior(CreateBehavior(new SpriteBehavior(*texManager->GetImage("player.png"), 16, 16, trans, window.get())));
	player->AddBehavior(CreateBehavior(new PlayerInputBehavior(trans)));
	
	currentLevel = Level::CreateLevelWithRooms(Level::LEVEL_SIZE::FINE, Level::Room::ROOM_SIZE::SMALL);

	//only for testing out proc gen
	//currentLevel.GetCamera().Zoom(5.00f);
	currentLevel.GetCamera().MoveCenter(0,0);
}

void ScionEngine::RenderFrame()
{
	std::stringstream ss;
	
	ss << currentLevel.GetCamera().GetPosition().x << ", " << currentLevel.GetCamera().GetPosition().y << endl;
	ss << currentLevel.GetCamera().GetTileBounds().left << ", " << currentLevel.GetCamera().GetTileBounds().top << " : " << currentLevel.GetCamera().GetTileBounds().width << ", " << currentLevel.GetCamera().GetTileBounds().height << endl;
	ss << currentLevel.GetCamera().GetZoom();

	sf::Text text(sf::String(ss.str()), *fontManager->LoadFromFile("Fonts/arial.ttf"));
	window->clear();
	stateManager->Draw(window.get());
	window->setView(window->getDefaultView());
	window->draw(text);
	window->display();
}

void ScionEngine::ProcessInput()
{
	//This input processor only deals with non keyboard and mouse presses
	events.clear();

	sf::Event evt;
	while(window->pollEvent(evt))
	{
		events.push_back(evt);

		switch(evt.type)
		{
		case sf::Event::Closed:
				window->close();
				break;
		case sf::Event::GainedFocus:
				isActive = true;
				break;
		case sf::Event::LostFocus:
				isActive = false;
				break;
		}
	}

	if(isActive)
	{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
	{
		auto oldCamera = currentLevel.GetCamera();

		currentLevel = Level::CreateLevelWithRooms(Level::LEVEL_SIZE::FINE, Level::Room::ROOM_SIZE::SMALL);

		currentLevel.GetCamera() = oldCamera;
	}
	}
} 

void ScionEngine::LoadImages()
{
	//preload images here
	texManager->GetImage("tiles.png");
	texManager->GetImage("player.png");
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

		if(isActive)
		{
			Update();
			RenderFrame();
		}
	}
}

void ScionEngine::Go()
{
	Init();

	GameLoop();
}

Entity* ScionEngine::CreateEntity()
{
	entitys.push_back(unique_ptr<Entity>(new Entity()));
	entitys.back()->game = this;

	return entitys.back().get();
}

Behavior* ScionEngine::CreateBehavior(Behavior* b)
{
	behaviors.push_back(unique_ptr<Behavior>(b));
	return b;
}
	
Attribute* ScionEngine::CreateAttribute(Attribute* a)
{
	attributes.push_back(unique_ptr<Attribute>(a));
	return a;
}