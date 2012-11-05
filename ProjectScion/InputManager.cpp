#include "InputManager.h"

#include <SFML\Window.hpp>

using namespace sf;

float InputManager::oldKeyState[NUM_KEYS], InputManager::newKeyState[NUM_KEYS];

void InputManager::Initialize()
{
    for(int i = 0; i < NUM_KEYS; i++)
    {
        oldKeyState[i] = 0.0f;
        newKeyState[i] = 0.0f;
    }
}

void InputManager::Update(sf::Window& win, float delta)
{
    Event evt;

    while(win.pollEvent(evt))
    {
        if(evt.type == Event::KeyPressed)
        {
            auto kp = newKeyState[evt.key.code];

            //accumulate held down time
            newKeyState[evt.key.code] = oldKeyState[evt.key.code] + delta;
        }
        else if(evt.type == Event::KeyReleased)
        {
            auto kp = newKeyState[evt.key.code];

            newKeyState[evt.key.code] = 0.0f;
        }
    }    
}

void InputManager::PostUpdate()
{
    memcpy(&oldKeyState, &newKeyState, sizeof(newKeyState));
}

bool InputManager::IsKeyPressed(int keycode)
{
    return oldKeyState[keycode] == 0 && newKeyState[keycode] > 0;
}

bool InputManager::IsKeyDown(int keycode)
{
    return newKeyState[keycode] > 0;
}

bool InputManager::IsKeyUp(int keycode)
{
    return oldKeyState[keycode] > 0 && newKeyState[keycode] == 0;
}