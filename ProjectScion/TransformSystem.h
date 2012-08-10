#ifndef _TRANSFORM_SYSTEM_H
#define _TRANSFORM_SYSTEM_H

#include "TransformComponent.h"
#include "ComponentSystem.h"
#include <vector>

class TransformSystem : public ComponentSystem
{
	std::vector<TransformComponent*> components;
public:
	void Process();
	void Initialize();
};

#endif