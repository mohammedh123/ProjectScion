#ifndef _C_SPRITE_H
#define _C_SPRITE_H

#include <string>

class CSprite : IComponent
{
public:
    CSprite(int param1);
    CSprite(int param1, std::string param2);
};

#endif