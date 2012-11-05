#ifndef _INPUT_SYSTEM_H
#define _INPUT_SYSTEM_H

#include "CInput.h"

class InputSystem : public IComponentSystem
{
	std::vector<std::unique_ptr<CInput>>
public:
	enum
	{
		INPUT
	};

    virtual void Update(float dt);
    virtual void RegisterEntity(Entity* ent) = 0;
    virtual void RegisterEntity(Entity* ent, int componentFlag, IComponent* componentPtr) = 0;
};

#endif