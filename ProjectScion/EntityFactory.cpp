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

    playerSprite->AddAnimation("left", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,0,32,32), 0, 1, 16, 16))
        .AddAnimation("up", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,32,32,32), 0, 1, 16, 16))
        .AddAnimation("right", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,64,32,32), 0, 1, 16, 16))
        .AddAnimation("down", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(32,96,32,32), 0, 1, 16, 16))
        .AddAnimation("left_moving", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,0,32,32), 0, 1, 16, 16))
        .AddAnimation("up_moving", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,32,32,32), 0, 1, 16, 16))
        .AddAnimation("right_moving", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,64,32,32), 0, 1, 16, 16))
        .AddAnimation("down_moving", sf::Animation(*ScionEngine::GetTexture("player.png"), sf::IntRect(0,96,32,32), 0, 1, 16, 16));
    playerSprite->SetCurrentAnimation("down");

    player->addComponent(playerSprite);
    player->addComponent(new CPlayer());
    player->addComponent(new CPosition(x, y));
    player->addComponent(new CDirection());

    player->activate();

    return player;
}

ac::es::Entity* EntityFactory::CreateTestEnemy(float x, float y, sf::RenderWindow* window)
{
    assert(gameScene != nullptr);

    auto enemy = gameScene->createEntity();
    auto enemySprite = new CSprite(window);

    enemySprite->AddAnimation("left", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(32,0,32,32), 0, 1, 16, 16))
        .AddAnimation("up", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(32,32,32,32), 0, 1, 16, 16))
        .AddAnimation("right", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(32,64,32,32), 0, 1, 16, 16))
        .AddAnimation("down", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(32,96,32,32), 0, 1, 16, 16))
        .AddAnimation("left_moving", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(0,0,32,32), 0, 1, 16, 16))
        .AddAnimation("up_moving", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(0,32,32,32), 0, 1, 16, 16))
        .AddAnimation("right_moving", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(0,64,32,32), 0, 1, 16, 16))
        .AddAnimation("down_moving", sf::Animation(*ScionEngine::GetTexture("enemy.png"), sf::IntRect(0,96,32,32), 0, 1, 16, 16));
    enemySprite->SetCurrentAnimation("down");

    //needs ai components
    enemy->addComponent(enemySprite);
    enemy->addComponent(new CPosition(x, y));
    enemy->addComponent(new CDirection());

    enemy->activate();

    return enemy;
}