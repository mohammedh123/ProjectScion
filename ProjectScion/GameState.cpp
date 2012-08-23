#include "GameState.h"
#include "StateManager.h"
#include "TextureManager.h"
#include <sstream>
#include "ScionEngine.h"
#include <noise/noise.h>
#include "noiseutils.h"
#include "SFML/OpenGL.hpp"

using namespace std;

void GameState::Initialize(ScionEngine* game)
{
	hoveredTile = nullptr;
	hoveredPosX = -1;
	hoveredPosY = -1;

	this->game = game;
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
	
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat light_position[] = { 0, 0, 0, 0.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
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

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		c.MoveBy(-1.0f,0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		c.MoveBy(1.0f,0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		c.MoveBy(0,-1.0f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		c.MoveBy(0,1.0f);

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), c.GetView());
		c.GoToCenter(MousePos.x, MousePos.y);
	}

	sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), c.GetView());
	
	sf::IntRect gameRect(0, 0, game->GetCurrentLevel().GetWidth()*Tile::SIZE, game->GetCurrentLevel().GetHeight()*Tile::SIZE);
	if(gameRect.contains(MousePos.x, MousePos.y))
	{
		hoveredTile = &game->GetCurrentLevel().GetTile(MousePos.x / Tile::SIZE, MousePos.y / Tile::SIZE);
		hoveredPosX = MousePos.x;
		hoveredPosY = MousePos.y;
	}
	else
	{
		hoveredTile = nullptr;
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
	window->setActive(true);
	//window->setView(*camera->GetView());
	/*
	glBegin(GL_TRIANGLES);
	glVertex3f( 0.0f, 300, 0.0f);		// Top
	glVertex3f(100.0f,100.0f, 0.0f);		// Bottom Left
	glVertex3f( 250.0f,50.0f, 0.0f);
	glEnd();
	*/
	Camera& c = game->GetCurrentLevel().GetCamera();

	//rt->clear(sf::Color::Transparent);
	//rt->setView(*camera->GetView());
	window->setView(c.GetView());
	

	//rt->display();
	//TextureDrawer->setTexture(rt->getTexture());
	//effect->setParameter("bgl_RenderedTexture", rt->getTexture());
	////TextureDrawer->setTexture(*WindowTexture.get());
 //   window->draw(*TextureDrawer.get(), *states.get());
	game->GetCurrentLevel().Draw(window);

	for(auto it = game->GetBehaviors().begin(); it != game->GetBehaviors().end(); it++)
	{
		if((*it)->IsRenderingBehavior())
			(*it)->Process();
	}

	if(hoveredTile)
	{
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(100, 100));
		rect.setFillColor(sf::Color(255,255,255,60));
		rect.setOutlineThickness(2.0f);
		rect.setOutlineColor(sf::Color::White);
		rect.setPosition(Tile::SIZE*int(hoveredPosX / Tile::SIZE), Tile::SIZE*int(hoveredPosY / Tile::SIZE));

		window->draw(rect);

		stringstream ss;
		ss << int(hoveredPosX / Tile::SIZE) << ", " << int(hoveredPosY / Tile::SIZE) << endl << hoveredTile->type;
		sf::Text t(ss.str(), *game->GetFont("Fonts/arial.ttf"), 24);
		t.setPosition(Tile::SIZE*int(hoveredPosX / Tile::SIZE), Tile::SIZE*int(hoveredPosY / Tile::SIZE));
		window->draw(t);
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
	
	
	window->setActive(false);
	if (currentState == TransitionOn)
	{
		stateManager->DrawSolidColor(sf::Color::Black, 1.0f - GetTransitionAlpha(), window);
	}
	else if (currentState == TransitionOff)
	{
		stateManager->DrawSolidColor(sf::Color::Black, GetTransitionAlpha(), window);
	}
}
