#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include <memory>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>

using namespace std;

template<class T>
class ResourceManager
{
private:
	
	std::map<std::string, std::unique_ptr<T>> contentMap;
	
	ResourceManager(const ResourceManager&);
	ResourceManager& operator=(const ResourceManager&);
public:
	ResourceManager();
	~ResourceManager();

	virtual T* Load(const std::string& filename);
};

#endif