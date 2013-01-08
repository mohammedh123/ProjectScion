#ifndef _ANIMATION_
#define _ANIMATION

#include <SFML\Graphics.hpp>

namespace sf
{
    class Animation : public Sprite
    {
        float _frameTime;
        sf::Clock _internalClock;
        sf::IntRect _originalRect;
        int _currentFrame, _numFrames;

        void SetFrame(int n);
    public:
        Animation();
        Animation(Texture& tex, IntRect texRect, float fps, int numFrames, float ox=0.0f, float oy=0.0f);

        void Update();
    };
};

#endif