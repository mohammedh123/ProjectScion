#ifndef _SOUND_BUFFER_MANAGER_H
#define _SOUND_BUFFER_MANAGER_H
 
#include <SFML/Audio.hpp>
 
#include "ResourceManager.h"
 
class SoundBufferManager : public ResourceManager< sf::SoundBuffer > {
public:
	SoundBufferManager():ResourceManager<sf::SoundBuffer>(){}
};

#endif

