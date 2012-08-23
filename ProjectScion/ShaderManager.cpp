#include "ShaderManager.h"
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}
sf::Shader* ShaderManager::LoadFromFile(const std::string & filename, sf::Shader::Type type)
{
	sf::Shader* retVal = nullptr;
	std::string key;
	if(type == sf::Shader::Type::Vertex)
	{
		key = filename + "+";
	}
	else//Fragment
	{
		key = "+" + filename;
	}
	
	for(auto it = begin(shaderMap); it != end(shaderMap); it++)
	{	
		if(it->first == key)
			retVal = it->second.get();
	}

	if(!retVal) //load new image and add it, then turn it
	{
		std::unique_ptr<sf::Shader> effect(new sf::Shader);
		effect->loadFromFile(filename, type);
		shaderMap[key] = std::move(effect);

		retVal = shaderMap[key].get();
	}

	return retVal;
}

sf::Shader* ShaderManager::LoadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	sf::Shader* retVal = nullptr;

	std::string key = vertexShaderFilename + "+" + fragmentShaderFilename;
	
	for(auto it = begin(shaderMap); it != end(shaderMap); it++)
	{	
		if(it->first == key)
			retVal = it->second.get();
	}

	if(!retVal) //load new image and add it, then turn it
	{
		std::unique_ptr<sf::Shader> effect(new sf::Shader);
		effect->loadFromFile(vertexShaderFilename, fragmentShaderFilename);
		shaderMap[key] = std::move(effect);

		retVal = shaderMap[key].get();
	}

	return retVal;
}
