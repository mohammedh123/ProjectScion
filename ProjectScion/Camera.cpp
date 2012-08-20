#include <SFML\Graphics.hpp>
#include <math.h>
#include "Camera.h"

using namespace std;

Camera::Camera(int w, int h, float speed, float depth) : speed(speed), depth(depth), view(sf::FloatRect(0,0,w, h))
{
	zoom = 1.0f;
	inverseZoom = 1.0f / zoom;
	
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

void Camera::Move(float x, float y)
{
	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::MoveBy(float x, float y)
{
	position.x += (float)x;
	position.y += (float)y;
	
	target.x = position.x;
	target.y = position.y;
}

void Camera::MoveCenter(float x, float y)
{
	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::GoTo(float x, float y)
{
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::Zoom(float z)
{
	float delta = 0;
	float upperBound = 6.0f;
	float lowerBound = .5f;

	if(zoom + z <= upperBound && zoom + z >= lowerBound)
	{
		zoom += z;
		view.zoom(z + 1);
	}

	inverseZoom = 1.0f / zoom;
}

void Camera::MoveRelative(float x, float y)
{
	view.move(x, y);
}

void Camera::GoToCenter(float x, float y)
{
	x = x;
	y = y;

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
	view.setCenter(position.x, position.y);
}

sf::IntRect Camera::GetTileBounds() const
{
	//0,0 is center
	auto viewSize = view.getSize();
	int offsetX = position.x - viewSize.x*0.5;
	int offsetY = position.y - viewSize.y*0.5;

	int x = (int)(offsetX / int(Tile::SIZE));
	int y = (int)(offsetY / int(Tile::SIZE));

	int w = (int)(viewSize.x / int(Tile::SIZE) + 2);
	int h = (int)(viewSize.y / int(Tile::SIZE) + 2);

	if(x % Tile::SIZE != 0)
		w++;
	if(y % Tile::SIZE != 0)
		h++;

	if(x < 0)
		w += x;
	if(y < 0)
		h += y;
	
	return sf::IntRect(std::max(x, 0), std::max(y, 0), w, h);
}

