#include "GameState.h"
#include "StateManager.h"
#include "ImageManager.h"

GameState::GameState()
{
}

void GameState::Initialize()
{
	State::Initialize();
	transitionOnTime = 0.5f;
	transitionOffTime = 0.5f;

	camera = std::unique_ptr<Camera>(new Camera(800, 600, 0.2f));

	currentLevel = std::unique_ptr<Level>(new Level(20, 10));
	for(int y = 0; y < currentLevel->GetHeight(); y++)
	{
		for(int x = 0; x < currentLevel->GetWidth(); x++)
		{
			if(y % 4 == 0)
				currentLevel->AddTile(x, y, new Tile(stateManager->imgManager->GetImage("tiles.png")));
			else
				currentLevel->AddTile(x, y, new Tile(stateManager->imgManager->GetImage("tiles2.png")));
		}
	}
	effect = stateManager->shaderManager->LoadFromFile("Shaders/bloom.frag", sf::Shader::Type::Fragment);
	rt = std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
	rt->create(800,600, true);
	rt->setView(*camera->GetView());
	//rt->setView(*camera->GetView());
	states = std::unique_ptr<sf::RenderStates>(new sf::RenderStates);
	states->shader = effect;
	
	WindowTexture = std::unique_ptr<sf::Texture>(new sf::Texture);
	TextureDrawer = std::unique_ptr<sf::Sprite>(new sf::Sprite);
}

void GameState::HandleInput(sf::RenderWindow* window)
{
	//sf::Event::MouseWheelMoved:
	//	camera->Zoom((evt->mouseWheel.delta > 0)?.5f:2.0f);
		
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		ExitState();
			
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
		camera->Zoom(-0.01f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
		camera->Zoom(.01f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		camera->MoveBy(-5.0f,0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		camera->MoveBy(5.0f,0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera->MoveBy(0,-5.0f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera->MoveBy(0,5.0f);

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		auto windowPosition = window->getPosition();
		sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition() - windowPosition, *camera->GetView());
		camera->GoToCenter(MousePos.x, MousePos.y);
	}
			
}

void GameState::Update(double delta, bool isGameActive, bool isCoveredByOtherState)
{
	State::Update(delta, isGameActive, isCoveredByOtherState);

	if (!isCoveredByOtherState && isGameActive)
	{
		camera->Update();
	}
}

void GameState::Draw(sf::RenderWindow* window)
{
	//window->setView(*camera->GetView());

	auto bounds = camera->GetTileBounds();
	auto camOffsetX = camera->GetTileOffset().x;
	auto camOffsetY = camera->GetTileOffset().y;

	
	rt->clear(sf::Color::Transparent);
	rt->setView(*camera->GetView());
	for(int y = 0, tileY = 0; y < bounds.height && tileY < currentLevel->GetHeight(); y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = 0; x < bounds.width && tileX < currentLevel->GetWidth(); x++, tileX++)
		{
		if(tileX < 0) continue;

			currentLevel->GetTile(tileX, tileY)->Draw(x*Tile::SIZE, y*Tile::SIZE, rt.get());
		}
	}
	
	rt->display();
	TextureDrawer->setTexture(rt->getTexture());
	effect->setParameter("bgl_RenderedTexture", rt->getTexture());
	//TextureDrawer->setTexture(*WindowTexture.get());
    window->draw(*TextureDrawer.get(), *states.get());
	//window->draw(*TextureDrawer.get());
	
	/*	
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
	
	*/

	if (currentState == TransitionOn)
	{
		stateManager->DrawSolidColor(sf::Color::Black, 1.0f - GetTransitionAlpha(), window);
	}
	else if (currentState == TransitionOff)
	{
		stateManager->DrawSolidColor(sf::Color::Black, GetTransitionAlpha(), window);
	}
}
