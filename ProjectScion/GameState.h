#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "State.h"
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "Camera.h"
#include "Level.h"
#include "Entity.h"

class GameState : public State
{
private:
	sf::Shader* effect;
	//Basically a render target where you render first and do some shader effects than draw it to the window
	std::unique_ptr<sf::RenderTexture> rt;
	//Basically your render state
	std::unique_ptr<sf::RenderStates> states;
	std::unique_ptr<sf::Texture> WindowTexture;
	std::unique_ptr<sf::Sprite> TextureDrawer;

	std::unique_ptr<Entity> player;

	ScionEngine* game;
public:
	GameState(ScionEngine* game);

	void Initialize();
	void HandleInput(sf::RenderWindow* window);
	void Update(double delta, bool isGameActive, bool isCoveredByOtherState);
	void Draw(sf::RenderWindow * window);
};

#endif