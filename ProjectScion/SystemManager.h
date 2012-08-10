#ifndef _SYSTEM_MANAGER_H
#define _SYSTEM_MANAGER_H

#include <map>
#include <string>
#include <memory>

#include "ComponentSystem.h"

class SystemManager
{
	std::map<std::string, std::unique_ptr<ComponentSystem>> systems;
	bool initialized;
public:
	SystemManager();
	
	template<class T> T* System(const std::string& name)
	{	
		T* retVal = nullptr;

		for(auto it = begin(systems); it != end(systems); it++)
		{
			if(it->first == name)
				retVal = reinterpret_cast<T*>(it->second.get());
		}

		if(!retVal)
		{			
			systems[name] = std::unique_ptr<T>(new T);

			retVal = reinterpret_cast<T*>(systems[name].get());
		}

		return retVal;
	}

	void Initialize();
};

#endif