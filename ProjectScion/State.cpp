#include "State.h"
#include "StateManager.h"

float State::GetTransitionAlpha() const
{
	return (1.0f - transitionPosition);
}

bool State::IsPopup() const
{
	return isPopup;
}

VisibleState State::getCurrentState() const
{
	return currentState;
}

void State::Initialize() 
{ 
	transitionOnTime = 0;
	transitionOffTime = 0;
	transitionPosition = 1;
	totalTime = 0;
	isPopup = false;
	isExiting = false;
	currentState = TransitionOn;
}

void State::Update(double delta, bool isGameActive, bool isCoveredByOtherState)
{
	if (isExiting)
	{
		// If the screen is going away to die, it should transition off.
		currentState = TransitionOff;

		if (!UpdateTransition(delta, transitionOffTime, 1))
		{
			// When the transition finishes, remove the screen.
			stateManager->RemoveState(this);
		}
	}
	else if (isCoveredByOtherState)
	{
		// If the screen is covered by another, it should transition off.
		if (UpdateTransition(delta, transitionOffTime, 1))
		{
			// Still busy transitioning.
			currentState = TransitionOff;
		}
		else
		{
			// Transition finished!
			currentState = Hidden;
		}
	}
	else
	{
		// Otherwise the screen should transition on and become active.
		if (UpdateTransition(delta, transitionOnTime, -1))
		{
			// Still busy transitioning.
			currentState = TransitionOn;
		}
		else
		{
			// Transition finished!
			currentState = Active;
		}
	}
}

bool State::UpdateTransition(double delta, double time, int direction)
{
	// How much should we move by?
	float transitionDelta;

	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = (float)(delta /
		time);

	// Update the transition position.
	transitionPosition += transitionDelta * direction;

	// Did we reach the end of the transition?
	if (((direction < 0) && (transitionPosition <= 0)) ||
		((direction > 0) && (transitionPosition >= 1)))
	{
		transitionPosition = transitionPosition < 0 ? 0 : (transitionPosition > 1 ? 1 : transitionPosition);
		return false;
	}

	// Otherwise we are still busy transitioning.
	return true;
}

void State::ExitState()
{
	if (transitionOffTime == 0)
	{
		// If the screen has a zero transition time, remove it immediately.
		stateManager->RemoveState(this);
	}
	else
	{
		// Otherwise flag that it should transition off and then exit.
		isExiting = true;
	}
}