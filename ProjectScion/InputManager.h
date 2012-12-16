#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <SFML\Window.hpp>

class InputManager
{
    static const int NUM_KEYS = 256;

    static float oldKeyState[NUM_KEYS], newKeyState[NUM_KEYS];
    static bool windowActive, windowClosed;

    static sf::Window* window;
public:
    static void Initialize(sf::Window* win);
    static void Update(float dt);
    static void PostUpdate();

    static bool IsKeyPressed(int keycode);
    static bool IsKeyDown(int keycode);
    static bool IsKeyUp(int keycode);

    static bool IsWindowClosed();
    static bool IsWindowActive();
};

#endif