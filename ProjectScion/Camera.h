#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML\Graphics.hpp>
#include "Tile.h"

class Camera
{
private:
	sf::Vector2f position;
	sf::Vector2f target;
	sf::Vector2i size;
	
	float depth;
	float speed;
	
public:

	std::unique_ptr<sf::View> view;

	Camera(int w, int h, float speed, float depth = 1.0f);
	~Camera();

	//goes to position immediately
	void Move(float x, float y);
	void MoveCenter(float x, float y);

	//'walks' to position
	void GoTo(float x, float y);
	void GoToCenter(float x, float y);

	void Zoom(float z);
	void MoveRelative(float x, float y);

	void Update();

	inline sf::Vector2i GetPosition() const { return sf::Vector2i((int)position.x, (int)position.y); }
	inline sf::Vector2i GetTileOffset() const { return sf::Vector2i((int)(position.x) % Tile::SIZE, (int)(position.y) % Tile::SIZE); }
	sf::IntRect GetTileBounds() const;
};

#endif

