#ifndef _LIGHT_H
#define _LIGHT_H

#include <SFML\Graphics.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace LIGHT_CONSTANTS
{
    static const int NUM_SEGMENTS = 32;
    static const float SEGMENT_ANGLE = (float)(2.0 * M_PI / NUM_SEGMENTS);
};

struct Light
{
    sf::Color color;
    float radius, bleed, linearizeFactor;

    Light(sf::Vector2f position, float radius, sf::Color color = sf::Color::White, float bleed = 1.0f, float linearizeFactor = 0.2f);
private:
    sf::Vector2f position;
    bool dirty;

    friend class LightManager;
};

#endif