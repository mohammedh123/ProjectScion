#include "ImageManager.h"
#include <vector>
#include <SFML\Graphics.hpp>

ImageManager::ImageManager()
{

}

ImageManager::~ImageManager()
{

}

void ImageManager::AddImage(const std::string& filename)
{
	std::unique_ptr<sf::Texture> tex(new sf::Texture);
	tex->loadFromFile(filename);

	imageList.push_back(std::move(tex));
}

sf::Texture* ImageManager::GetImage(int index) const
{
	return imageList[index].get();
}