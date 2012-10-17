#ifndef _TRANSFORM_ATTR_H
#define _TRANSFORM_ATTR_H

#include "Attribute.h"
#include "Entity.h"

#include <SFML\System.hpp>

enum TRANS_DIR
{
	LEFT,
	UP,
	DOWN,
	RIGHT
};

class TransformAttribute : public Attribute
{
	sf::Vector2f _position, _velocity, _scale;
	float _angle, _spriteAngle;
public:
	TRANS_DIR Direction;

	TransformAttribute(sf::Vector2f& position, 
		sf::Vector2f& velocity, 
		sf::Vector2f& scale = sf::Vector2f(1.0f, 1.0f), 
		float angle = 0.0f, 
		float spriteAngle = 0.0f);

	TransformAttribute(float x, 
		float y, 
		float vx, 
		float vy, 
		float sx=1.0f, 
		float sy=1.0f, 
		float angle=0.0f, 
		float spriteAngle=0.0f);
	
	void Move(float dx, float dy);

	void MoveTo(float x, float y);

	inline	const	sf::Vector2f&	GetPosition()	const { return _position; }
	inline	const	sf::Vector2f&	GetScale()		const { return _scale; }
	inline			float			GetAngle()		const { return _angle; }
	inline			float			GetTotalAngle() const { return _angle+_spriteAngle; }
	inline			TRANS_DIR		GetDirection()	const { return Direction; }
};

#endif