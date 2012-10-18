#ifndef _IMAGE_MANAGER_H
#define _IMAGE_MANAGER_H

#include <memory>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>

class TextureManager
{
private:
    std::map<std::string, std::unique_ptr<sf::Texture>> imageMap;
    
    TextureManager(const TextureManager&);
    TextureManager& operator=(const TextureManager&);
public:
    TextureManager();
    ~TextureManager();

    sf::Texture* TextureManager::GetImage(const std::string& filename);
};

#endif