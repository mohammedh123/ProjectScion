#include "InputManager.h"

#include <SFML\Window.hpp>

using namespace sf;

float InputManager::oldKeyState[NUM_KEYS], InputManager::newKeyState[NUM_KEYS];
sf::Window* InputManager::window = nullptr;
bool InputManager::windowActive, InputManager::windowClosed;

void InputManager::Initialize(sf::Window* win)
{
    InputManager::window = win;

    for(int i = 0; i < NUM_KEYS; i++)
    {
        oldKeyState[i] = 0.0f;
        newKeyState[i] = 0.0f;
    }
}

void InputManager::Update(float dt)
{
    Event evt;

    while(window->pollEvent(evt))
    {
        if(evt.type == Event::KeyPressed)
        {
            auto kp = newKeyState[evt.key.code];

            //accumulate held down time
            newKeyState[evt.key.code] = oldKeyState[evt.key.code] + dt;
        }
        else if(evt.type == Event::KeyReleased)
        {
            auto kp = newKeyState[evt.key.code];

            newKeyState[evt.key.code] = 0.0f;
        }
        else if(evt.type == Event::Closed)
        {
            windowClosed = true;
        }
        else if(evt.type == Event::GainedFocus)
        {
            windowActive = true;
        }
        else if(evt.type == Event::LostFocus)
        {
            windowActive = false;
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

bool InputManager::IsWindowClosed()
{
    return windowClosed;
}

bool InputManager::IsWindowActive()
{
    return windowActive;
}