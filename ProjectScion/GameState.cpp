#include "GameState.h"
#include "StateManager.h"
#include "TextureManager.h"

#include "ScionEngine.h"

using namespace std;

void GameState::Initialize(ScionEngine* game)
{
	State::Initialize();

	transitionOnTime = 0.5f;
	transitionOffTime = 0.5f;

	effect = game->GetShader("Shaders/bloom.frag", sf::Shader::Type::Fragment);

	//player = move(unique_ptr<Entity>(new Entity()));
	
	auto c = game->GetCurrentLevel().GetCamera();

	rt = unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
	rt->create(800,600, true);
	//rt->setView(*camera->GetView());
	rt->setView(c.GetView());
	states = unique_ptr<sf::RenderStates>(new sf::RenderStates);
	states->shader = effect;
	
	WindowTexture = unique_ptr<sf::Texture>(new sf::Texture);
	TextureDrawer = unique_ptr<sf::Sprite>(new sf::Sprite);
}

void GameState::HandleInput(sf::RenderWindow* window)
{
	//sf::Event::MouseWheelMoved:
	//	camera->Zoom((evt->mouseWheel.delta > 0)?.5f:2.0f);

	Camera& c = game->GetCurrentLevel().GetCamera();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		ExitState();
			
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
		c.Zoom(-0.01f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
		c.Zoom(.01f);

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//	camera->MoveBy(-5.0f,0);

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//	camera->MoveBy(5.0f,0);

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	//	camera->MoveBy(0,-5.0f);

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	//	camera->MoveBy(0,5.0f);

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		auto windowPosition = window->getPosition();
		sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition() - windowPosition, c.GetView());
		c.GoToCenter(MousePos.x, MousePos.y);
	}
			
}

void GameState::Update(double delta, bool isGameActive, bool isCoveredByOtherState)
{
	Camera& c = game->GetCurrentLevel().GetCamera();
	State::Update(delta, isGameActive, isCoveredByOtherState);

	if (!isCoveredByOtherState && isGameActive)
	{
		c.Update();
	}

	for(auto it = game->GetBehaviors().begin(); it != game->GetBehaviors().end(); it++)
	{
		if(!(*it)->IsRenderingBehavior())
			(*it)->Process();
	}
}

void GameState::Draw(sf::RenderWindow* window)
{
	//window->setView(*camera->GetView());

	
	Camera& c = game->GetCurrentLevel().GetCamera();

	//rt->clear(sf::Color::Transparent);
	//rt->setView(*camera->GetView());
	window->setView(c.GetView());
	

	//rt->display();
	//TextureDrawer->setTexture(rt->getTexture());
	//effect->setParameter("bgl_RenderedTexture", rt->getTexture());
	////TextureDrawer->setTexture(*WindowTexture.get());
 //   window->draw(*TextureDrawer.get(), *states.get());
	for(auto it = game->GetBehaviors().begin(); it != game->GetBehaviors().end(); it++)
	{
		if((*it)->IsRenderingBehavior())
			(*it)->Process();
	}
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
