#ifndef _IMAGE_MANAGER_H
#define _IMAGE_MANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <SFML\Graphics.hpp>

class ImageManager
{
private:
	std::vector<std::unique_ptr<sf::Texture>> imageList;
public:
	ImageManager();
	~ImageManager();

	void AddImage(const std::string& filename);
	sf::Texture* ImageManager::GetImage(int index) const;
};

#endif