#include "SplashScreenState.h"
#include "StateManager.h"
#include "ScionEngine.h"
#include "SFML/Graphics/Shader.hpp"

SplashScreenState::SplashScreenState(float duration, sf::Texture* image, sf::SoundBuffer* soundBuffer, bool clickToSkip):duration(duration),State()
{
    rect.setTexture(image, true);
    rect.setPosition(0,0);
    
    runTime = 0;
    sound.setBuffer(*soundBuffer);
}

void SplashScreenState::Initialize(ScionEngine* game)
{
    State::Initialize();
    transitionOnTime = 0.5f;
    transitionOffTime = 0.5f;
    sound.play();
    effect = game->GetShader("Shaders/bloom.frag", sf::Shader::Type::Fragment);
}

void SplashScreenState::Update(double delta, bool isGameActive, bool isCoveredByOtherState)
{
    
    State::Update(delta, isGameActive, isCoveredByOtherState);

    if(std::rand() % 10 == 0)
        rect.setPosition((((std::rand() % 2) == 0)?-1.0f:1.0f )* runTime * 20 , (((std::rand() % 2) == 0)?-1:1) * runTime * 20);
    runTime += delta;
    if(runTime > duration)
        ExitState();
}

void SplashScreenState::Draw(sf::RenderWindow* window)
{
    auto size = window->getSize();
    rect.setSize(sf::Vector2f(size.x, size.y));
    effect->setParameter("bgl_RenderedTexture",*rect.getTexture());

    sf::RenderStates states;
    states.shader = effect;
    window->draw(rect, states);
}

void SplashScreenState::HandleInput(sf::RenderWindow* window)
{
}