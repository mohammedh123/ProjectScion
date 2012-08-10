#include "SystemManager.h"
#include <algorithm>

using namespace std;

SystemManager::SystemManager() : initialized(false)
{
}

void SystemManager::Initialize()
{
	for(auto it = systems.begin(); it != systems.end(); it++)
	{
		it->second->Initialize();
	}
}