#ifndef _UI_SCREEN_H
#define _UI_SCREEN_H

#include "SFML/Graphics.hpp"

enum UIScreenState
{
	ENTER, EXIT, NORMAL, HIDDEN
};

class UIScreen
{
private:
	UIScreenState screenState;
	sf::RectangleShape _transitionRect;
public:
	UIScreen();
	virtual void Update(float delta) = 0;
	virtual void Draw(sf::RenderWindow* window) = 0;
	virtual void OnEnter();
	virtual void OnExit();
};

#endif