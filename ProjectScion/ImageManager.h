#ifndef _IMAGE_MANAGER_H
#define _IMAGE_MANAGER_H

#include <memory>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>

class ImageManager
{
private:
	std::map<std::string, std::unique_ptr<sf::Texture>> imageMap;
	
	ImageManager(const ImageManager&);
	ImageManager& operator=(const ImageManager&);
public:
	ImageManager();
	~ImageManager();

	sf::Texture* ImageManager::GetImage(const std::string& filename);
};

#endif