#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <SFML\Window.hpp>

class InputManager
{
    static const int NUM_KEYS = 256;

    static float oldKeyState[NUM_KEYS], newKeyState[NUM_KEYS];
public:
    static void Initialize();
    static void Update(sf::Window& win, float delta);
    static void PostUpdate();

    static bool IsKeyPressed(int keycode);
    static bool IsKeyDown(int keycode);
    static bool IsKeyUp(int keycode);
};

#endif