#ifndef _COMPONENT_I_H
#define _COMPONENT_I_H

// Mohammed Hossain
// IComponent is an interface that all future components will inherit from. Components should be PODs with public data in them.
// IComponents should simply be "state". Systems should act on the state. For example:
//      CSprite: contains data about the entity's sprite: direction/angle, which sprite, origin, etc
//      CInput: updates its state about the entity's current direction, angle, etc, all based on input (whether input be AI or from the keyboard)

class IComponent
{
public:
    friend class IComponentSystem;
};

#endif