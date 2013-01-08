#include "ScionEngine.h"
#include "LevelGenerator.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"
#include "PlayerSystem.h"
#include "CollisionSystem.h"
#include "AttackSystem.h"
#include "InputManager.h"
#include "EntityFactory.h"

#include <string>
#include <sstream>
#include <windows.h>

using namespace std;

std::vector<sf::Event>    ScionEngine::events(8);
StateManager*            ScionEngine::stateManager(new StateManager());
TextureManager*            ScionEngine::texManager(new TextureManager());
SoundBufferManager*        ScionEngine::soundBufferManager(new SoundBufferManager());
FontManager*            ScionEngine::fontManager(new FontManager());
MusicManager*            ScionEngine::musicManager(new MusicManager());
ShaderManager*            ScionEngine::shaderManager(new ShaderManager());
std::mt19937            ScionEngine::randEngine(GetTickCount());
float                   ScionEngine::dt;

ScionEngine::ScionEngine()
{
    isActive = true;
}

ScionEngine::~ScionEngine()
{
    delete texManager;
    delete soundBufferManager;
    delete fontManager;
    delete musicManager;
    delete shaderManager;
    delete stateManager;
}

void ScionEngine::Init()
{
    EntityFactory::gameScene = &gameScene;
    const auto windowWidth = 800;
    const auto windowHeight = 600;

    window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight, 32), "Project Scion"));
    window->setFramerateLimit(60);

    InputManager::Initialize(window.get());

    texManager = new TextureManager();
    soundBufferManager = new SoundBufferManager();
    fontManager = new FontManager();    
    musicManager = new MusicManager();
    shaderManager = new ShaderManager();
    stateManager = new StateManager();

    fonts["mainFont"] = fontManager->LoadFromFile("Fonts/arial.ttf");
    stateManager->PushState(new GameState(), this);
    //stateManager->PushState(new SplashScreenState(4.0f, texManager->GetImage("Images/splashscreen.png"), soundBufferManager->LoadFromFile("Sound/splash_sound.wav")));
    LoadImages();
    clock = unique_ptr<sf::Clock>(new sf::Clock());
    clock->restart();

    ScionEngine::events.reserve(8);
        
    currentLevel = std::move(LevelGenerator::CreateLevelWithRooms1(Level::FINE, ROOM::SMALL));

    const Tile* randTile = currentLevel.GetRandomTileOfType(Tile::GROUND);

    //only for testing out proc gen
    auto fstZ = float(currentLevel.GetWidth()*Tile::SIZE)/windowWidth;
    auto sndZ = float(currentLevel.GetHeight()*Tile::SIZE)/windowHeight;
    currentLevel.GetCamera().DirectZoomOfOriginal(max(fstZ, sndZ)+0.1f);
    //currentLevel.GetCamera().MoveCenter(0, 0);
    currentLevel.GetCamera().MoveCenter(currentLevel.GetWidth()*Tile::SIZE/2.0f, currentLevel.GetHeight()*Tile::SIZE/2.0f);
    
    gfxSys = new GraphicsSystem();
    gfxSys->setIsPassive(true);
    gameScene.insertEntitySystem(gfxSys);
    gameScene.insertEntitySystem(new LogicSystem(&dt));
    gameScene.insertEntitySystem(new PlayerSystem(&dt));

    auto player = EntityFactory::CreatePlayer(randTile->x*Tile::SIZE + Tile::SIZE*0.5f, randTile->y*Tile::SIZE + Tile::SIZE*0.5f, window.get());
    gameScene.initialize();
    gameScene.clean();
}

void ScionEngine::RenderFrame()
{
    sf::Vector2f MousePos = window->convertCoords(  sf::Mouse::getPosition(*window), GetCurrentLevel().GetCamera().GetView());
    std::stringstream ss;
    
    ss << currentLevel.GetCamera().GetPosition().x << ", " << currentLevel.GetCamera().GetPosition().y << endl;
    ss << currentLevel.GetCamera().GetTileBounds().left << ", " << currentLevel.GetCamera().GetTileBounds().top << " : " << currentLevel.GetCamera().GetTileBounds().width << ", " << currentLevel.GetCamera().GetTileBounds().height << endl;
    ss << currentLevel.GetCamera().GetZoom() << endl;
    ss << MousePos.x << ", " << MousePos.y;

    sf::Text text(sf::String(ss.str()), *fontManager->LoadFromFile("Fonts/arial.ttf"));
    window->clear();
    stateManager->Draw(window.get());
    window->setView(window->getDefaultView());
    window->draw(text);
    window->display();
}

void ScionEngine::ProcessInput(float dt)
{
    InputManager::Update(dt);

    if(InputManager::IsWindowActive())
    {
        if(InputManager::IsKeyPressed(sf::Keyboard::K))
        {
            auto oldCamera = currentLevel.GetCamera();
        
            currentLevel = std::move(LevelGenerator::CreateLevelWithRooms1(Level::FINE, ROOM::SMALL));

            currentLevel.GetCamera() = oldCamera;
        }
    }
} 

void ScionEngine::LoadImages()
{
    //preload images here
    texManager->GetImage("tiles.png");
    texManager->GetImage("newtiles.png");
    texManager->GetImage("player.png");
}

void ScionEngine::Update(float dt)
{
    ScionEngine::dt = dt;
    clock->restart();
    stateManager->Update(dt, window.get());
}

void ScionEngine::GameLoop()
{
    while(window->isOpen())
    {
        float dt = clock->getElapsedTime().asSeconds();
        ProcessInput(dt);

        if(isActive)
        {
            Update(dt);
            RenderFrame();
        }

        InputManager::PostUpdate();
    }
}

void ScionEngine::Go()
{
    Init();

    GameLoop();
}