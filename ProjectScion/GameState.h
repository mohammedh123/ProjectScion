#include "State.h"
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "Camera.h"
#include "Level.h"

class GameState : public State
{
private:
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Level> currentLevel;
public:
	GameState();
	void Initialize();

	void HandleInput(sf::RenderWindow* window);

	void Update(double delta, bool isGameActive, bool isCoveredByOtherState);

	void Draw(sf::RenderWindow * window);
};