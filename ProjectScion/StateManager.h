#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

#include <list>
#include "SFML/Graphics.hpp"
#include "Managers.h"

class State;
class ScionEngine;

class StateManager
{
private:
    std::list<State*> States;
    std::list<State*> statesToUpdate;

    
    
    StateManager(const StateManager&);
    StateManager& operator=(const StateManager&);
    //GameManager game = null;
    sf::RectangleShape rect;
public:    
    StateManager() { }
    ~StateManager();
    //State[] GetStates();

    void PushState(State* state, ScionEngine* game=0);

    void RemoveState(State* state);

    State* PopState();

    void Update(double delta, sf::RenderWindow* window);

    void Draw(sf::RenderWindow* window);

    void DrawSolidColor(sf::Color color, float alpha, sf::RenderWindow* window);
};

#endif