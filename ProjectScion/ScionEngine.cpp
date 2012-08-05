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
	{
		for(int x = 0; x < currentLevel->GetWidth(); x++)
		{
			if(y % 4 == 0)
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles.png")));
			else
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles2.png")));
		}
	}
	
	LoadImages();
}

void ScionEngine::RenderFrame()
{
	window->clear();
	
	auto bounds = camera->GetTileBounds();
	auto camOffsetX = camera->GetTileOffset().x;
	auto camOffsetY = camera->GetTileOffset().y;

	for(int y = 0, tileY = 0; y < bounds.height && tileY < currentLevel->GetHeight(); y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = 0; x < bounds.width && tileX < currentLevel->GetWidth(); x++, tileX++)
		{
		if(tileX < 0) continue;

			currentLevel->GetTile(tileX, tileY)->Draw(x*Tile::SIZE, y*Tile::SIZE, window.get());
		}
	}
	
	
	window->setView(*camera->GetView());

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
		case sf::Event::MouseWheelMoved:
			camera->Zoom((evt.mouseWheel.delta > 0)?.5f:2.0f);
			break;
		case sf::Event::KeyPressed:
			switch(evt.key.code)
			{
			case sf::Keyboard::PageUp:
				camera->Zoom(0.5f);
				break;
			case sf::Keyboard::PageDown:
				camera->Zoom(2.0f);
				break;
			case sf::Keyboard::Left:
				camera->MoveBy(-5.0f,0);
				break;
			case sf::Keyboard::Right:
				camera->MoveBy(5.0f,0);
				break;
			case sf::Keyboard::Up:
				camera->MoveBy(0,-5.0f);
				break;
			case sf::Keyboard::Down:
				camera->MoveBy(0,5.0f);
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			sf::Vector2f MousePos = window->convertCoords( sf::Vector2i( evt.mouseButton.x,  evt.mouseButton.y), *camera->GetView());
			camera->GoToCenter(MousePos.x, MousePos.y);
			
			
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