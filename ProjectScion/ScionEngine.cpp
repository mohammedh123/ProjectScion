//HI
#include "ScionEngine.h"

ScionEngine::ScionEngine()
{
}

ScionEngine::~ScionEngine()
{
	delete currentLevel;
}

void ScionEngine::Init()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(640, 480, 32), "Project Scion"));												
	imgManager = std::unique_ptr<ImageManager>(new ImageManager());
	camera = std::unique_ptr<Camera>(new Camera(640, 480, 0.2f));

	currentLevel = new Level(20, 10);
	for(int y = 0; y < currentLevel->GetHeight(); y++)
		for(int x = 0; x < currentLevel->GetWidth(); x++)
		{
			if(y % 4 == 0)
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles.png")));
			else
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles2.png")));
		}

	LoadImages();
}

void ScionEngine::RenderFrame()
{
	window->clear();
	
	auto bounds = camera->GetTileBounds();
	auto camOffsetX = camera->GetTileOffset().x;
	auto camOffsetY = camera->GetTileOffset().y;

	for(int y = 0, tileY = bounds.top; y < bounds.height && tileY < currentLevel->GetHeight(); y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = bounds.left; x < bounds.width && tileX < currentLevel->GetWidth(); x++, tileX++)
		{
		if(tileX < 0) continue;

			currentLevel->GetTile(tileX, tileY)->Draw(x*Tile::SIZE - camOffsetX, y*Tile::SIZE - camOffsetY, window.get());
		}
	}

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
			break;
		case sf::Event::MouseButtonPressed:
			auto pos = sf::Mouse::getPosition(*window);
			camera->GoToCenter(pos.x + camera->GetPosition().x, pos.y + camera->GetPosition().y);
			break;
		}
	}
} 

void ScionEngine::LoadImages()
{
	//preload images here
	imgManager->GetImage("tiles.png");
}

void ScionEngine::Update()
{
	camera->Update();
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