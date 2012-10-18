#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H
 
#include <SFML/Graphics.hpp>
 
#include "ResourceManager.h"
 
class FontManager : public ResourceManager< sf::Font > {
public:
    FontManager():ResourceManager<sf::Font>(){}
};

#endif

