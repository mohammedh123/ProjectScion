#include "CSprite.h"

CSprite::CSprite(sf::RenderWindow* window) : windowToDrawIn(window)
{
}
    
CSprite& CSprite::AddAnimation(const std::string& label, sf::Animation& animation)
{
    animations[label] = animation;
    return *this;
}

void CSprite::SetCurrentAnimation(const std::string& label)
{
    currentAnimation = label;
}
