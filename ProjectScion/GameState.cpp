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
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles.png")));
			else
				currentLevel->AddTile(x, y, new Tile(imgManager->GetImage("tiles2.png")));
		}
	}
}

void GameState::HandleInput(sf::Event* evt, sf::RenderWindow* window)
{
	switch(evt->type)
	{
	case sf::Event::KeyPressed:
		switch(evt->key.code)
		{
		case sf::Event::MouseWheelMoved:
			camera->Zoom((evt->mouseWheel.delta > 0)?.5f:2.0f);
			break;
		case sf::Keyboard::Escape:
			ExitState();
			break;
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
			sf::Vector2f MousePos = window->convertCoords( sf::Vector2i( evt->mouseButton.x,  evt->mouseButton.y), *camera->GetView());
			camera->GoToCenter(MousePos.x, MousePos.y);
			break;
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

			currentLevel->GetTile(tileX, tileY)->Draw(x*Tile::SIZE, y*Tile::SIZE, window);
		}
	}

	if (currentState == TransitionOn)
	{
		stateManager->DrawSolidColor(sf::Color::Black, 1.0f - GetTransitionAlpha(), window);
	}
	else if (currentState == TransitionOff)
	{
		stateManager->DrawSolidColor(sf::Color::Black, GetTransitionAlpha(), window);
	}
}
