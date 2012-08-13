#include "StateManager.h"	
#include "State.h"

#include <algorithm>

StateManager::~StateManager()
{
	std::for_each(begin(States), end(States), [](State* state){
		delete state;
	});
}

void StateManager::PushState(State* state, ScionEngine* game)
{
	state->stateManager = this;
	state->Initialize(game);
	
	States.push_back(state);
}

void StateManager::RemoveState(State* state)
{
	States.remove(state);
	statesToUpdate.remove(state);

	delete state;
}

State* StateManager::PopState()
{
	State* state = nullptr;

	if(States.size() > 0)
	{
		state = States.back();
		States.pop_back();
	}

	return state;
}

void StateManager::Update(double delta, sf::RenderWindow* window)
{
	statesToUpdate.clear();

	for(std::list<State*>::iterator it = States.begin(); it != States.end(); ++it)
	{
		statesToUpdate.push_back(*it);
	}
	bool isGameActive = true;
	//bool isGameActive = game.IsActive;
	bool isCoveredByOtherState = false;
	State* currentState;

	while(statesToUpdate.size() > 0)
	{
		currentState = statesToUpdate.back();

		if (isGameActive)
			currentState->HandleInput(window);

		currentState->Update(delta, isGameActive, isCoveredByOtherState);

		isCoveredByOtherState = !currentState->IsPopup();

		statesToUpdate.pop_back();
	}
	std::list<State*>::iterator it = States.begin();
	for( ; it != States.end(); )
	{
		std::list<State*>::iterator temp = (it);
		it++;
		if((*temp)->IsDead())
		{
			States.remove(*temp);
		}
		
	}
}

void StateManager::Draw(sf::RenderWindow* window)
{
	for(std::list<State*>::iterator it = States.begin(); it != States.end(); ++it)
	{
		if ((*it)->getCurrentState() == Hidden)
			continue;

		(*it)->Draw(window);
	}
}

void StateManager::DrawSolidColor(sf::Color c, float alpha, sf::RenderWindow* window)
{
	rect.setFillColor(c);
	rect.setPosition(0,0);
	auto size = window->getSize();
	rect.setSize(sf::Vector2f((float)size.x, (float)size.y));
	window->draw(rect);
}
