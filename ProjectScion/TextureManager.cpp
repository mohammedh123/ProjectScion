#include "TextureManager.h"
#include <vector>
#include <SFML\Graphics.hpp>

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

sf::Texture* TextureManager::GetImage(const std::string& filename)
{
	sf::Texture* retVal = nullptr;

	for(auto it = begin(imageMap); it != end(imageMap); it++)
	{
		if(it->first == filename)
			retVal = it->second.get();
	}

	if(!retVal) //load new image and add it, then turn it
	{
		std::unique_ptr<sf::Texture> tex(new sf::Texture);
		tex->loadFromFile(filename);
		imageMap[filename] = std::move(tex);

		retVal = imageMap[filename].get();
	}

	return retVal;
}