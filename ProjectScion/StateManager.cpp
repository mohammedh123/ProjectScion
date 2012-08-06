#include "StateManager.h"	
#include "State.h"
#include <algorithm>

StateManager::StateManager()
{
	States = new std::list<State*>();
	statesToUpdate = new std::list<State*>();
}
StateManager::~StateManager()
{
	States->clear();
	statesToUpdate->clear();
	delete States;
	delete statesToUpdate;
}
void StateManager::PushState(State* state)
{
	//state->stateManager = this;
	state->Initialize();
	States->push_back(state);
}

void StateManager::RemoveState(State* state)
{
	States->remove(state);
	statesToUpdate->remove(state);
}

State* StateManager::PopState()
{
	State* state = 0;

	if(States->size() > 0)
	{
		state = States->back();
		States->pop_back();
	}

	return state;
}

void StateManager::PopAll()
{
	while (PopState() != 0);
}

void StateManager::Update(double delta, sf::Event evt)
{
	statesToUpdate->clear();

	for(std::list<State*>::iterator it = States->begin(); it != States->end(); ++it)
	{
		statesToUpdate->push_back(*it);
	}
	bool isGameActive = true;
	//bool isGameActive = game.IsActive;
	bool isCoveredByOtherState = false;
	State* currentState;

	while(statesToUpdate->size() > 0)
	{
		currentState = statesToUpdate->back();

		if (isGameActive)
			currentState->HandleInput(evt);

		currentState->Update(delta, isGameActive, isCoveredByOtherState);


		isCoveredByOtherState = !currentState->IsPopup();

		statesToUpdate->remove(currentState);
	}
}

void StateManager::Draw(sf::RenderWindow& window)
{
	for(std::list<State*>::iterator it = States->begin(); it != States->end(); ++it)
	{
		if ((*it)->getCurrentState() == Hidden)
			continue;

		(*it)->Draw(window);
	}
}
/*
void DrawSolidColor(Color c, float alpha, RenderTarget2D target = null)
{
	Viewport viewport = graphicsDevice.Viewport;

	//spriteBatch.Begin();

	spriteBatch.Draw(blankTexture,
		new Rectangle(0, 0, viewport.Width, viewport.Height),
		Color.Black * alpha);

	//spriteBatch.End();
}*/
