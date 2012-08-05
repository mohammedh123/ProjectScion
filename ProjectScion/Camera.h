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

	float speed;
public:
	Camera(int w, int h, float speed);
	~Camera();

	//goes to position immediately
	void Move(int x, int y);
	void MoveCenter(int x, int y);

	//'walks' to position
	void GoTo(int x, int y);
	void GoToCenter(int x, int y);

	void Update();

	inline sf::Vector2i GetPosition() const { return sf::Vector2i((int)position.x, (int)position.y); }
	inline sf::Vector2i GetTileOffset() const { return sf::Vector2i((int)(position.x) % Tile::SIZE, (int)(position.y) % Tile::SIZE); }
	sf::IntRect GetTileBounds() const;
};

#endif

