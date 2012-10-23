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
    //lm = unique_ptr<LightManager>(new LightManager(game));
    //lm->AddLight(new Light(sf::Vector2f(386,300), 100.0f, sf::Color::Red));
    //lm->AddLight(new Light(sf::Vector2f(0,250), 100.0f, sf::Color::Green));
    //lm->AddLight(new Light(sf::Vector2f(800,300), 100.0f, sf::Color::Green));
    //lm->AddLight(new Light(sf::Vector2f(0,600), 100.0f, sf::Color::Green));

    hoveredTile = nullptr;
    hoveredPosX = -1;
    hoveredPosY = -1;

    this->game = game;
    State::Initialize();

    transitionOnTime = 0.5f;
    transitionOffTime = 0.5f;

    //effect = game->GetShader("Shaders/bloom.frag", sf::Shader::Type::Fragment);
    //lightFX = game->GetShader("Shaders/light.frag", sf::Shader::Type::Fragment);
    darkFX = game->GetShader("Shaders/dark.frag", sf::Shader::Fragment);
    darkFX->setParameter("screenWidth", 800);
    darkFX->setParameter("screenHeight", 600);
    darkFX->setParameter("lightStrength", 0.5f);
    darkFX->setParameter("lightColor", sf::Color::White);
    darkFX->setParameter("lightRadius", 200.0f);

    combineFX = game->GetShader("Shaders/combine.frag", sf::Shader::Fragment);
    combineFX->setParameter("lightAmbient", 4);
    combineFX->setParameter("ambient", 0.5f);
    combineFX->setParameter("ambientColor", sf::Color::Green);

    //player = move(unique_ptr<Entity>(new Entity()));
    
    auto c = game->GetCurrentLevel().GetCamera();
    
    //lightRT = unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
    //lightRT->create(800,600, false);
    //darkRT = unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
    //darkRT->create(800,600, false);
    //colorRT = unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
    //colorRT->create(800,600, false);

    states = unique_ptr<sf::RenderStates>(new sf::RenderStates);
    //states->shader = effect;
    
    WindowTexture = unique_ptr<sf::Texture>(new sf::Texture);
    TextureDrawer = unique_ptr<sf::Sprite>(new sf::Sprite);
    
    //glClearDepth(1);

    //glEnable(GL_DEPTH_TEST);
    //glShadeModel (GL_SMOOTH);
 //   glEnable(GL_LIGHTING);
 //   glEnable(GL_LIGHT0);
 //   
 //   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
 //   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
 //   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 //   GLfloat mat_shininess[] = { 100.0 };
 //   GLfloat light_position[] = { 0, 0, 0, 0.0 };

 //   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 //   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 //   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
 //   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
 //   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
}

void GameState::HandleInput(sf::RenderWindow* window)
{
    //sf::Event::MouseWheelMoved:
    //    camera->Zoom((evt->mouseWheel.delta > 0)?.5f:2.0f);

    Camera& c = game->GetCurrentLevel().GetCamera();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        ExitState();
            
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
        c.Zoom(-0.01f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
        c.Zoom(.01f);
    /*
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        c.MoveBy(-1.0f,0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        c.MoveBy(1.0f,0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        c.MoveBy(0,-1.0f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        c.MoveBy(0,1.0f);
    */
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), c.GetView());
        c.GoToCenter(MousePos.x, MousePos.y);
    }

    sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), c.GetView());
    auto mousePos = sf::Mouse::getPosition(*window);

    sf::IntRect gameRect(0, 0, game->GetCurrentLevel().GetWidth()*Tile::SIZE, game->GetCurrentLevel().GetHeight()*Tile::SIZE);
    if(gameRect.contains((int)MousePos.x, (int)MousePos.y))
    {
        hoveredTile = &game->GetCurrentLevel().GetTile((int)(MousePos.x / Tile::SIZE), (int)(MousePos.y / Tile::SIZE));
        hoveredPosX = (int)MousePos.x;
        hoveredPosY = (int)MousePos.y;
        
        darkFX->setParameter("lightPosition", sf::Vector2f(MousePos.x, MousePos.y));
        
        //lightSystem->SetView(c.GetView());
        //testLight->SetCenter(Vec2f(400, 300));
        //testLight->SetCenter(Vec2f(MousePos.x, MousePos.y));
    }
    else
    {
        hoveredTile = nullptr;
    }
}

void GameState::Update(float delta, bool isGameActive, bool isCoveredByOtherState)
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

    //lm->Update();
}

void GameState::Draw(sf::RenderWindow* window)
{ 
    window->setActive(true);
    Camera& c = game->GetCurrentLevel().GetCamera();
    

    window->clear(sf::Color::Black);
    window->setView(c.GetView());

    //colorRT->setView(c.GetView());
    game->GetCurrentLevel().Draw(window);

    for(auto it = game->GetBehaviors().begin(); it != game->GetBehaviors().end(); it++)
    {
        if((*it)->IsRenderingBehavior())
            (*it)->Process();
    }

    window->setView(window->getDefaultView());
    //window->setView(c.GetView());
    
    //lm->SetView(c.GetView());
    //lm->Draw(window);

    hoveredTile = false;
    if(hoveredTile)
    {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(100, 100));
        rect.setFillColor(sf::Color(255,255,255,60));
        rect.setOutlineThickness(2.0f);
        rect.setOutlineColor(sf::Color::White);
        rect.setPosition(Tile::SIZE*int(hoveredPosX / Tile::SIZE)*1.0f, Tile::SIZE*int(hoveredPosY / Tile::SIZE)*1.0f);

        window->draw(rect);

        stringstream ss;
        ss << int(hoveredPosX / Tile::SIZE) << ", " << int(hoveredPosY / Tile::SIZE) << endl << hoveredTile->type;
        sf::Text t(ss.str(), *game->GetFont("Fonts/arial.ttf"), 24);
        t.setPosition(Tile::SIZE*int(hoveredPosX / Tile::SIZE)*1.0f, Tile::SIZE*int(hoveredPosY / Tile::SIZE)*1.0f);
        window->draw(t);
    }
    
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
