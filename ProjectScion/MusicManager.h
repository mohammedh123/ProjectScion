#ifndef _MUSIC_MANAGER_H
#define _MUSIC_MANAGER_H
 
#include <memory>
#include <map>
#include <string>
#include <SFML/Audio.hpp>

 
class MusicManager
{
private:
	std::map<std::string, std::unique_ptr<sf::Music>> musicMap;
	
	MusicManager(const MusicManager&);
	MusicManager& operator=(const MusicManager&);
public:
	MusicManager();
	~MusicManager();

	sf::Music* MusicManager::LoadFromFile(const std::string& filename);
};

#endif

