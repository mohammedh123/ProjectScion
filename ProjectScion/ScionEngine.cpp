#include "ScionEngine.h"

#include "SpriteBehavior.h"
#include "PlayerInputBehavior.h"

using namespace std;

std::vector<sf::Event> ScionEngine::events(8);

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
	stateManager->LoadResourceManager(imgManager.get(), soundBufferManager.get(), fontManager.get(), musicManager.get(), shaderManager.get());
		
	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
	stateManager->PushState(new GameState(this));
	//stateManager->PushState(new SplashScreenState(4.0f, imgManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	LoadImages();
	clock = unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();

	ScionEngine::events.reserve(8);

	auto player = CreateEntity();
	TransformAttribute* trans = static_cast<TransformAttribute*>(CreateAttribute(new TransformAttribute(0, 0, 0, 0)));
	player->AddBehavior(CreateBehavior(new SpriteBehavior(*imgManager->GetImage("player.png"), 16, 16, trans, window.get())));
	player->AddBehavior(CreateBehavior(new PlayerInputBehavior(trans)));
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