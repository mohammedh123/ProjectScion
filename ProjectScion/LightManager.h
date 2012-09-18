#ifndef _LIGHT_MANAGER_H
#define _LIGHT_MANAGER_H

#include <vector>
#include <memory>

#include "Light.h"

class ScionEngine;
class Level;

#include <SFML/Graphics.hpp>

class LightManager
{
	Level& level;

	sf::Shader* lightAttenuationShader;
	sf::RenderTexture lightTexture;
	sf::Sprite lightSprite;

	std::vector<std::unique_ptr<Light>> lights;
public:
	LightManager(ScionEngine* game);

	void Update();

	void AddLight(Light* light);

	void Draw(sf::RenderWindow* window);
	void DrawLights();
};

#endif