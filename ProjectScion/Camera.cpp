#include <SFML\Graphics.hpp>
#include <math.h>
#include "Camera.h"

Camera::Camera(int w, int h, float speed) : speed(speed)
{
	size.x = w;
	size.y = h;

	if(speed < 0.0)
		speed = 0.0;
	if(speed > 1.0)
		speed = 1.0;
}

Camera::~Camera()
{
}

void Camera::Move(int x, int y)
{
	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::MoveCenter(int x, int y)
{
	x = x - (size.x / 2);
	y = y - (size.y / 2);

	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::GoTo(int x, int y)
{
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::GoToCenter(int x, int y)
{
	x = x - (size.x / 2);
	y = y - (size.y / 2);

	target.x = (float)x;
	target.y = (float)y;
}

void Camera::Update()
{
	float x, y, d;

	float vx, vy, v;

	x = (float)(target.x - position.x);
	y = (float)(target.y - position.y);

	if((x*x + y*y) <= 1)
	{
		position.x = target.x;
		position.y = target.y;
	}
	else
	{
		d = sqrt(x*x + y*y);
		v = (d * speed)/60;

		if(v < 1.0f)
			v = 1.0f;
		
		vx = x * (v/d);
		vy = y * (v/d);

		position.x += vx;
		position.y += vy;
	}
}

sf::IntRect Camera::GetTileBounds() const
{
	int x = (int)(position.x / Tile::SIZE);
	int y = (int)(position.y / Tile::SIZE);

	int w = (int)(size.x / Tile::SIZE + 2);
	int h = (int)(size.y / Tile::SIZE + 2);

	if(x % Tile::SIZE != 0)
		w++;
	if(y % Tile::SIZE != 0)
		h++;

	return sf::IntRect(x, y, w, h);
}

