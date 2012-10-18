#include "MusicManager.h"
#include <vector>
#include <SFML\Audio.hpp>

MusicManager::MusicManager()
{

}

MusicManager::~MusicManager()
{

}

sf::Music* MusicManager::LoadFromFile(const std::string& filename)
{
    sf::Music* retVal = nullptr;

    for(auto it = begin(musicMap); it != end(musicMap); it++)
    {
        if(it->first == filename)
            retVal = it->second.get();
    }

    if(!retVal) //load new image and add it, then turn it
    {
        std::unique_ptr<sf::Music> tex(new sf::Music);
        tex->openFromFile(filename);
        musicMap[filename] = std::move(tex);

        retVal = musicMap[filename].get();
    }

    return retVal;
}