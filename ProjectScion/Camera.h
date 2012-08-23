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
	
	float zoom, inverseZoom;
	float depth;
	float speed;

	sf::View view;
	sf::Vector2i originalDimensions;
public:
	Camera(int w, int h, float speed, float depth = 1.0f);
	~Camera();

	//goes to position immediately
	void Move(float x, float y);
	void MoveBy(float x, float y);
	void MoveCenter(float x, float y);

	//'walks' to position
	void GoTo(float x, float y);
	void GoToCenter(float x, float y);
	
	void Zoom(float z);
	void DirectZoomOfOriginal(float z);
	void MoveRelative(float x, float y);

	void Update();

	inline const sf::View& GetView() const { return view;}
	
	inline sf::Vector2i GetPosition() const { return sf::Vector2i((int)position.x, (int)position.y); }
	inline float GetZoom() const { return zoom; }
	inline sf::Vector2i GetTileOffset() const { return sf::Vector2i((int)(position.x) % Tile::SIZE, (int)(position.y) % Tile::SIZE); }
	sf::IntRect GetTileBounds() const;
};

#endif

