#ifndef _STATE_H
#define _STATE_H

#include "SFML\Graphics.hpp"
#include "SoundBufferManager.h"
#include "FontManager.h"
#include "ImageManager.h"

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
    
    
	
protected:
	VisibleState        currentState;
    double              transitionOnTime;
    double              transitionOffTime;
    float               transitionPosition;
	sf::Keyboard		previousKeyboardState;
	sf::Keyboard		currentKeyboardState;
public:
	/*
    double       getTotalTime();
    bool         getIsPopup();
    bool         getIsExiting();
    VisibleState getCurrentState();
    double       getTransitionOnTime();
    double       getTransitionOffTime();
    float        getTransitionPosition();
    float        getTransitionAlpha();

	void		 setTotalTime(double newTotalTime);
    void         setIsPopup(bool newIsPopUp);
    void         setIsExiting(bool newIsExiting);
    void		 setCurrentState(VisibleState newState);
    void         setTransitionOnTime(double newTime);
    void         setTransitionOffTime(double newTime);
    void         setTransitionPosition(float newPosition);
    void         setTransitionAlpha(float newAlpha);
	*/


	State::~State(){}
	
	StateManager* stateManager;
	ImageManager* imgManager; 
	SoundBufferManager* soundBufferManager; 
	FontManager* fontManager;

	bool IsPopup() const;
	float GetTransitionAlpha() const;
	VisibleState getCurrentState() const;

	virtual void LoadResourceManager(ImageManager* iM, SoundBufferManager* sBM, FontManager* fM);

    virtual void Initialize();

    virtual void HandleInput(sf::Event* evt, sf::RenderWindow* window) = 0;

    virtual void Update(double delta, bool isGameActive, bool isCoveredByOtherState);

    bool UpdateTransition(double delta, double time, int direction);

    virtual void Draw(sf::RenderWindow* window) = 0;

    void ExitState();
};

#endif