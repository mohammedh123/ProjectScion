#ifndef _SOUND_BUFFER_MANAGER2_H
#define _SOUND_BUFFER_MANAGER2_H

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Audio\SoundBuffer.hpp>

class SoundBufferManager2
{
private:
	
	std::map<std::string, std::unique_ptr<sf::SoundBuffer>> contentMap;
	/*
	ResourceManager(const ResourceManager&);
	ResourceManager& operator=(const ResourceManager&);
	*/
public:
	SoundBufferManager2(){}

	sf::SoundBuffer* LoadFromFile(const std::string& filename)
	{
		sf::SoundBuffer* retVal = nullptr;

		for(auto it = begin(contentMap); it != end(contentMap); it++)
		{
			if(it->first == filename)
				retVal = it->second.get();
		}

		if(!retVal)
		{
			std::unique_ptr<sf::SoundBuffer> tex(new sf::SoundBuffer);
			tex->loadFromFile(filename);
			contentMap[filename] = std::move(tex);

			retVal = contentMap[filename].get();
		}

		return retVal;
	}
};

#endif