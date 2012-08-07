#ifndef _UI_SPLASH_SCREEN_H
#define _UI_SPLASH_SCREEN_H

#include "SFML/Graphics.hpp"
#include "UIScreen.h"
#include "SFML/Audio.hpp"

class SplashScreen : public UIScreen
{
private:
	sf::RectangleShape rect;
	float duration;
	float runTime;
	sf::Sound sound;
public:
	SplashScreen(float duration, sf::Texture* image, sf::SoundBuffer* soundBuffer, bool clickToSkip = false);
	virtual void Update(float delta);
	virtual void Draw(sf::RenderWindow& window);
	bool Finished(){return runTime >= duration;}
};

#endif