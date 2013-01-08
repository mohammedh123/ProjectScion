#include "EntityFactory.h"
#include "ScionEngine.h"

#include "Components.h"

#include <cassert>
#include <ac/es.h>

ac::es::Scene* EntityFactory::gameScene = nullptr;

ac::es::Entity* EntityFactory::CreatePlayer(float x, float y, sf::RenderWindow* window)
{
    assert(gameScene != nullptr);

    auto player = gameScene->createEntity();
    auto playerSprite = new CSprite(window);

    playerSprite->AddAnimation("right", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,0,32,32), 0, 1, 16, 16))
        .AddAnimation("down", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,0,32,32), 0, 1, 16, 16))
        .AddAnimation("up", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,32,32,32), 0, 1, 16, 16))
        .AddAnimation("left", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,32,32,32), 0, 1, 16, 16));
    playerSprite->SetCurrentAnimation("up");

    player->addComponent(playerSprite);
    player->addComponent(new CPlayer());
    player->addComponent(new CPosition(x, y));

    player->activate();

    return player;
}