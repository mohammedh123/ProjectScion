#include "Light.h"

Light::Light(sf::Vector2f position, float radius, sf::Color color, float bleed, float linearizeFactor) : 
	position(position), radius(radius), color(color), bleed(bleed), linearizeFactor(linearizeFactor), dirty(false)
{

}