#include "ScionEngine.h"

ScionEngine::ScionEngine()
{
}

ScionEngine::~ScionEngine()
{
	
}

void ScionEngine::Init()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Project Scion"));		

	imgManager = std::unique_ptr<ImageManager>(new ImageManager());
	soundBufferManager = std::unique_ptr<SoundBufferManager>(new SoundBufferManager());
	fontManager = std::unique_ptr<FontManager>(new FontManager());
	stateManager = std::unique_ptr<StateManager>(new StateManager());
	stateManager->LoadResourceManager(imgManager.get(), soundBufferManager.get(), fontManager.get());
	
	fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
	stateManager->PushState(new GameState());
	//splashScreen = std::unique_ptr<SplashScreen>(new SplashScreen(4.0f, imgManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	stateManager->PushState(new SplashScreenState(4.0f, imgManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
	
	evt = new sf::Event();
	LoadImages();
	clock = std::unique_ptr<sf::Clock>(new sf::Clock());
	clock->restart();
}

void ScionEngine::RenderFrame()
{
	window->clear();
	stateManager->Draw(window.get());
	/*
	switch(currentGameState)
	{

	case GS_SPLASH_SCREEN:
		splashScreen->Draw(window.get());
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
	*/
	window->display();
}

void ScionEngine::ProcessInput()
{
	//The event handler they have is bad, for simplicity only one state can handle event at a time	
	window->pollEvent(*evt);
	switch(evt->type)
	{
	case sf::Event::Closed:
            window->close();
            break;
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
	stateManager->Update(delta, *evt, window.get());
	/*
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
	*/
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