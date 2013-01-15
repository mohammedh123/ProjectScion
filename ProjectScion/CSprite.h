#ifndef _C_SPRITE_H
#define _C_SPRITE_H

#include "Animation.h"

#include <map>
#include <ac/es.h>

using namespace sf;

class CSprite : public ac::es::Component<CSprite>
{
public:
    std::map<const std::string, sf::Animation> animations;
    std::string currentAnimation;
    sf::RenderWindow* windowToDrawIn;

    CSprite(sf::RenderWindow* window);

    CSprite& AddAnimation(const std::string& label, sf::Animation& animation);
    void SetCurrentAnimation(const std::string& label);
    inline sf::Animation& GetCurrentAnimation()
    {
        return animations[currentAnimation];
    }
};

#endif