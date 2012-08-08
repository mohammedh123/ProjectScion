#include <list>
#include "SFML/Graphics.hpp"
#include "SoundBufferManager.h"
#include "FontManager.h"
#include "ImageManager.h"

class State;

class StateManager
{
private:
	std::list<State*>* States;
	std::list<State*>* statesToUpdate;

	
	
	StateManager(const StateManager&);
	StateManager& operator=(const StateManager&);
	//GameManager game = null;
	sf::RectangleShape rect;

public:
	ImageManager* imgManager; 
	SoundBufferManager* soundBufferManager; 
	FontManager* fontManager;

	StateManager();
	~StateManager();
	void LoadResourceManager(ImageManager* iM, SoundBufferManager* sBM, FontManager* fM);
	//State[] GetStates();

	void PushState(State* state);

	void RemoveState(State* state);

	State* PopState();

	void PopAll();

	void Update(double delta, sf::RenderWindow* window);

	void Draw(sf::RenderWindow* window);

	void DrawSolidColor(sf::Color color, float alpha, sf::RenderWindow* window);
};