#include "SplashScreen.h"

SplashScreen::SplashScreen(float duration, sf::Texture* image, sf::SoundBuffer* soundBuffer, bool clickToSkip):duration(duration)
{
	rect.setTexture(image, true);
	rect.setPosition(0,0);
	rect.setSize(sf::Vector2f(640.0f, 480.0f));
	runTime = 0;
	sound.setBuffer(*soundBuffer);
	sound.play();
}

void SplashScreen::Update(float delta)
{
	//nice random function to shake the splash screen
	if(std::rand() % 10 == 0)
		rect.setPosition((((std::rand() % 2) == 0)?-1.0f:1.0f )* runTime * 20 , (((std::rand() % 2) == 0)?-1:1) * runTime * 20);
	runTime += delta;
}

void SplashScreen::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(rect);
}