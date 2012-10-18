#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

template<class T>
class ResourceManager
{
private:
    ResourceManager(const ResourceManager&);
    ResourceManager& operator=(const ResourceManager&);
    
protected:
    std::map<std::string, std::unique_ptr<T>> contentMap;
public:
    ResourceManager<T>(){}
    ~ResourceManager<T>(){}

    virtual T* LoadFromFile(const std::string& filename)
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
};

#endif