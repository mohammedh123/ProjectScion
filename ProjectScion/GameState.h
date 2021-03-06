#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "SFML/Graphics.hpp"

#include "State.h"
#include "Tile.h"
#include "Camera.h"
#include "Level.h"
#include "LightManager.h"
#include <ac/es.h>

class GameState : public State
{
private:
    sf::Shader* darkFX, *lightFX, *combineFX;
    //Basically a render target where you render first and do some shader effects than draw it to the window
    //std::unique_ptr<sf::RenderTexture> darkRT, lightRT, colorRT;
    //Basically your render state
    std::unique_ptr<sf::RenderStates> states;
    std::unique_ptr<sf::Texture> WindowTexture;
    std::unique_ptr<sf::Sprite> TextureDrawer;
    
    std::unique_ptr<ac::es::Entity> player;

    ScionEngine* game;

    Tile* hoveredTile;
    //std::unique_ptr<LightManager> lm;
    int hoveredPosX, hoveredPosY;
public:
    void Initialize(ScionEngine* game);
    void HandleInput(sf::RenderWindow* window);
    void Update(double delta, bool isGameActive, bool isCoveredByOtherState);
    void Draw(sf::RenderWindow * window);
};

#endif