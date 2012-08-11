#ifndef _STATE_H
#define _STATE_H

#include "SFML\Graphics.hpp"
#include "Managers.h"

class StateManager;

enum VisibleState
{
    TransitionOn,
    Active,
    TransitionOff,
    Hidden,
};

class State
{
private:
    double              totalTime;
    bool                isPopup;
    bool                isExiting;
    bool				isDead;
    
	
protected:
	VisibleState        currentState;
    double              transitionOnTime;
    double              transitionOffTime;
    float               transitionPosition;
public:
	State::~State(){}
	
	StateManager* stateManager;

	bool IsDead() const;
	bool IsPopup() const;
	float GetTransitionAlpha() const;
	VisibleState getCurrentState() const;

    virtual void Initialize();

    virtual void HandleInput(sf::RenderWindow* window) = 0;

    virtual void Update(double delta, bool isGameActive, bool isCoveredByOtherState);

    bool UpdateTransition(double delta, double time, int direction);

    virtual void Draw(sf::RenderWindow* window) = 0;

    void ExitState();
};

#endif