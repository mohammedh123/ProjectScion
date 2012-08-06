#include "State.h"
#include <list>
#include "SFML/Graphics.hpp"

class StateManager
{
private:
	std::list<State*>* States;
	std::list<State*>* statesToUpdate;
	
	static StateManager *instance;

	StateManager();
	~StateManager();
	StateManager(const StateManager&);
	StateManager& operator=(const StateManager&);
	//GameManager game = null;
	//Texture2D blankTexture;

public:
	//access StateManager via StateManager.Instance
	static StateManager* Instance()
	{
		if (instance == 0)
		{
			instance = new StateManager();
		}

		return instance;
	}

	//to avoid accidental construction
	

	//State[] GetStates();

	void PushState(State* state);

	void RemoveState(State* state);

	State* PopState();

	void PopAll();

	void Update(double delta, sf::Event evt);

	void Draw(sf::RenderWindow& window);

	void DrawSolidColor(sf::Color color, float alpha, sf::RenderWindow& window);
};