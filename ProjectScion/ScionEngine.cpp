#include "ScionEngine.h"
#include "SpriteBehavior.h"
#include "PlayerInputBehavior.h"
#include "LevelGenerator.h"
#include <string>
#include <sstream>
#include <windows.h>
#include "PlayerCollisionBehavior.h"


using namespace std;

std::vector<sf::Event>					ScionEngine::events(8);
StateManager*							ScionEngine::stateManager(new StateManager());
TextureManager*						ScionEngine::texManager(new TextureManager());
SoundBufferManager*					ScionEngine::soundBufferManager(new SoundBufferManager());
FontManager*			ScionEngine::fontManager(new FontManager());
MusicManager*			ScionEngine::musicManager(new MusicManager());
ShaderManager*			ScionEngine::shaderManager(new ShaderManager());
std::mt19937							ScionEngine::randEngine(GetTickCount());

ScionEngine::ScionEngine()
{
	isActive = true;
}

ScionEngine::~ScionEngine()
{
	delete texManager;
	delete soundBufferManager;
	delete fontManager;
	delete musicManager;
	delete shaderManager;
	delete stateManager;
}

void ScionEngine::Init()
{
	const auto windowWidth = 800;
	const auto windowHeight = 600;

	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight, 32), "Project Scion"));
	window->setFramerateLimit(60);

	texManager = new TextureManager();
	soundBufferManager = new SoundBufferManager();
	fontManager = new FontManager();
	
	musicManager = new MusicManager();
	shaderManager = new ShaderManager();
	stateManager = new StateManager();

	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
	stateManager->PushState(new GameState(), this);
	//stateManager->PushState(new SplashScreenState(4.0f, texManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	LoadImages();
	clock = unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();

	ScionEngine::events.reserve(8);
		
	currentLevel = std::move(LevelGenerator::CreateLevelWithRooms1(Level::SIZE::FINE, ROOM::SIZE::SMALL));

	auto player = CreateEntity();
	const Tile* randTile = currentLevel.GetRandomTileOfType(Tile::GROUND);
	TransformAttribute* trans = static_cast<TransformAttribute*>(CreateAttribute(new TransformAttribute(randTile->x*Tile::SIZE + Tile::SIZE*0.5f, randTile->y*Tile::SIZE + Tile::SIZE*0.5f, 0, 0)));
	player->AddBehavior(CreateBehavior(new SpriteBehavior(*texManager->GetImage("player.png"), 16, 16, trans, window.get())));
	player->AddBehavior(CreateBehavior(new PlayerInputBehavior(trans)));
	player->AddBehavior(CreateBehavior(new PlayerCollisionBehavior(*texManager->GetImage("player.png"), 16, 16, trans, currentLevel)));
	//only for testing out proc gen
	auto fstZ = float(currentLevel.GetWidth()*Tile::SIZE)/windowWidth;
	auto sndZ = float(currentLevel.GetHeight()*Tile::SIZE)/windowHeight;
	currentLevel.GetCamera().DirectZoomOfOriginal(max(fstZ, sndZ)+0.1f);
	//currentLevel.GetCamera().MoveCenter(0, 0);
	currentLevel.GetCamera().MoveCenter(currentLevel.GetWidth()*Tile::SIZE/2, currentLevel.GetHeight()*Tile::SIZE/2);
}
void ScionEngine::RenderFrame()
{
	sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), GetCurrentLevel().GetCamera().GetView());
	std::stringstream ss;
	
	ss << currentLevel.GetCamera().GetPosition().x << ", " << currentLevel.GetCamera().GetPosition().y << endl;
	ss << currentLevel.GetCamera().GetTileBounds().left << ", " << currentLevel.GetCamera().GetTileBounds().top << " : " << currentLevel.GetCamera().GetTileBounds().width << ", " << currentLevel.GetCamera().GetTileBounds().height << endl;
	ss << currentLevel.GetCamera().GetZoom() << endl;
	ss << MousePos.x << ", " << MousePos.y;

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
		
			currentLevel = std::move(LevelGenerator::CreateLevelWithRooms1(Level::SIZE::FINE, ROOM::SIZE::SMALL));

			currentLevel.GetCamera() = oldCamera;
		}
	}
} 

void ScionEngine::LoadImages()
{
	//preload images here
	texManager->GetImage("tiles.png");
	texManager->GetImage("newtiles.png");
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