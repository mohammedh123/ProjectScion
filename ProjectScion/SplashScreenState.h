#ifndef _SPLASH_SCREEN_STATE_H
#define _SPLASH_SCREEN_STATE_H

#include "State.h"
#include "SFML/Graphics.hpp"
#include "UIScreen.h"
#include "SFML/Audio.hpp"

class SplashScreenState : public State
{
private:
    sf::RectangleShape rect;
    float duration;
    float runTime;
    sf::Sound sound;
    sf::Shader* effect;
public:
    SplashScreenState(float duration, sf::Texture* image, sf::SoundBuffer* soundBuffer, bool clickToSkip = false);
    virtual void Initialize(ScionEngine* game);
    virtual void Update(float delta, bool isGameActive, bool isCoveredByOtherState);
    virtual void Draw(sf::RenderWindow* window);
    virtual void HandleInput(sf::RenderWindow* window);
};

#endif
