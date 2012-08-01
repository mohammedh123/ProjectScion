#include "ScionEngine.h"

ScionEngine::ScionEngine()
{
}

ScionEngine::~ScionEngine()
{
}

void ScionEngine::Init()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(640, 480, 32), "Project Scion"));												
}

void ScionEngine::RenderFrame()
{
	window->clear();
	window->display();
}

void ScionEngine::ProcessInput()
{
	sf::Event evt;

	while(window->pollEvent(evt))
	{
		switch(evt.type)
		{
		case sf::Event::Closed:
			window->close();
		}
	}
}

void ScionEngine::Update()
{
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