#ifndef _BEHAVIOR_H
#define _BEHAVIOR_H

#include <map>
#include <string>

#include "Attribute.h"

class Behavior
{
public:
	Behavior(){}
	Behavior(std::map<std::string, Attribute*> attribs){}

	virtual void Process() = 0;
	virtual bool IsRenderingBehavior() const { return false; }
};

#endif
