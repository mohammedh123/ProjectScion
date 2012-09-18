#include "LightManager.h"
#include "ScionEngine.h"

#include <SFML\OpenGL.hpp>

LightManager::LightManager(ScionEngine* game) : 
	level(game->GetCurrentLevel())
{
	lightTexture.create(game->GetWindow()->getSize().x, game->GetWindow()->getSize().y);
	lightSprite.setTexture(lightTexture.getTexture());

	lightAttenuationShader = ScionEngine::GetShader("Shaders/lightAttenuationShader.frag", sf::Shader::Fragment);
}

void LightManager::Update()
{
	bool anyDirty = false;

	for(int i = 0; i < lights.size(); i++)
		if(lights[i]->dirty)
		{
			anyDirty = true;
			lights[i]->dirty = false;
		}

	if(anyDirty)
		DrawLights();
}

void LightManager::AddLight(Light* light)
{
	lights.emplace_back(light);
	light->dirty = true;
}

void LightManager::DrawLights()
{	
	sf::VertexArray verts(sf::PrimitiveType::TrianglesFan, LIGHT_CONSTANTS::NUM_SEGMENTS+2);
	
	lightTexture.clear(sf::Color::Transparent);
		
		//	
		//glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

		//sf::Vector2u size(lightTexture.getSize());
		//float width = static_cast<float>(size.x);
		//float height = static_cast<float>(size.y);

		//glBlendFunc(GL_ONE, GL_ZERO);

		//glBegin(GL_QUADS);
		//	glVertex2f(0.0f, 0.0f);
		//	glVertex2f(width, 0.0f);
		//	glVertex2f(width, height);
		//	glVertex2f(0.0f, height);
		//glEnd();
		//
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	for(int i = 0; i < lights.size(); i++)
	{
		verts[0] = lights[i]->position;
		
		for(int j = 0; j <= LIGHT_CONSTANTS::NUM_SEGMENTS; j++)
		{
			verts[j+1].position = sf::Vector2f(lights[i]->radius * cosf(LIGHT_CONSTANTS::SEGMENT_ANGLE*j),
				lights[i]->radius * sinf(LIGHT_CONSTANTS::SEGMENT_ANGLE*j));
			verts[j+1].position += lights[i]->position;
		}
			
		lightAttenuationShader->setParameter("lightPos", lights[i]->position.x, 300 + (300 - lights[i]->position.y));
		lightAttenuationShader->setParameter("lightColor", sf::Vector3f(lights[i]->color.r/255.0f, lights[i]->color.g/255.0f, lights[i]->color.b/255.0f));
		lightAttenuationShader->setParameter("radius", lights[i]->radius);
		lightAttenuationShader->setParameter("bleed", lights[i]->bleed);
		lightAttenuationShader->setParameter("linearizeFactor", lights[i]->linearizeFactor);

		sf::RenderStates states;
		lightTexture.draw(verts, lightAttenuationShader);

		//glBlendFunc(GL_ONE, GL_ONE);
	}

	lightTexture.display();
}

void LightManager::Draw(sf::RenderWindow* window)
{
	lightSprite.setTexture(lightTexture.getTexture());
	window->draw(lightSprite);
}