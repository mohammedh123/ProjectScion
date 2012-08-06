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
	soundBufferManager = std::unique_ptr<SoundBufferManager>(new SoundBufferManager());
	fontManager = std::unique_ptr<FontManager>(new FontManager());
	
	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
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
	splashScreen = std::unique_ptr<SplashScreen>(new SplashScreen(4.0f, imgManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	currentGameState = GS_SPLASH_SCREEN;

	LoadImages();
	clock = std::unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();
}

void ScionEngine::RenderFrame()
{
	window->clear();
	
	switch(currentGameState)
	{
	case GS_SPLASH_SCREEN:
		splashScreen->Draw(*window);
		break;
	case GS_GAME:
		//Set the View that the game will be drawn in
		window->setView(*camera->GetView());

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
	
		// Set the default view back
		sf::View defaultView = window->getDefaultView();
		window->setView(window->getDefaultView());
	
		//Draw the UI
		//Needs to be encapsulated and thought out

		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(defaultView.getSize().x, 100));
		rectangle.setPosition(0, defaultView.getSize().y - 100);
		window->draw(rectangle);

		sf::Text text("UI");
		text.setFont(*fonts["mainFont"]);
		text.setCharacterSize(30);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Red);
		text.setPosition(defaultView.getSize().x/2, defaultView.getSize().y - 50);
		//Does not have a built in alignment option =/
		window->draw(text);
		break;
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
	float delta = clock->getElapsedTime().asSeconds();
	clock->restart();

	switch(currentGameState)
	{
	case GS_SPLASH_SCREEN:
		splashScreen->Update(delta);
		if(splashScreen->Finished())
		{
			currentGameState = GS_GAME;
		}
		
		break;
	case GS_GAME:
		camera->Update();
	}
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