#include <list>
#include "SFML/Graphics.hpp"
#include "Managers.h"

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
	//move these from being public to being accessed via static GetImage, GetSound, etc
	ImageManager* imgManager; 
	SoundBufferManager* soundBufferManager; 
	FontManager* fontManager;
	MusicManager* musicManager;
	ShaderManager* shaderManager;
	
	StateManager();
	~StateManager();
	void LoadResourceManager(ImageManager* iM, SoundBufferManager* sBM, FontManager* fM, MusicManager *mM, ShaderManager* sM);
	//State[] GetStates();

	void PushState(State* state);

	void RemoveState(State* state);

	State* PopState();

	void PopAll();

	void Update(double delta, sf::RenderWindow* window);

	void Draw(sf::RenderWindow* window);

	void DrawSolidColor(sf::Color color, float alpha, sf::RenderWindow* window);
};