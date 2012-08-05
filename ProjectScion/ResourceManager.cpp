#include "ResourceManager.h"
#include <vector>
#include <SFML\Graphics.hpp>

template<class T>
ResourceManager<T>::ResourceManager()
{

}
template<class T>
ResourceManager<T>::~ResourceManager()
{

}

template<class T>
T* ResourceManager<T>::Load(const std::string& filename)
{
	T* retVal = nullptr;

	for(auto it = begin(contentMap); it != end(contentMap); it++)
	{
		if(it->first == filename)
			retVal = it->second.get();
	}

	if(!retVal)
	{
		std::unique_ptr<T> tex(new T);
		tex->loadFromFile(filename);
		contentMap[filename] = std::move(tex);

		retVal = contentMap[filename].get();
	}

	return retVal;
}