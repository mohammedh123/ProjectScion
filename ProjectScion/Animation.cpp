#include "Animation.h"

using namespace sf;

Animation::Animation()
{
    _frameTime = 0;
}

Animation::Animation(Texture& tex, IntRect texRect, float fps, int numFrames, float ox, float oy) : Sprite(tex, texRect), _currentFrame(0), _originalRect(texRect), _numFrames(numFrames)
{
    if(fps != 0)
        _frameTime = (1.0f/fps);
    else
        _frameTime = 1000.0f;

    Sprite::setOrigin(ox, oy);
}

void Animation::Update()
{
    if(_internalClock.getElapsedTime().asSeconds() >= _frameTime)
    {
        //go to next frame
        SetFrame(_currentFrame + 1);
        _internalClock.restart();
    }
}

void Animation::SetFrame(int n)
{
    static const auto textureSize = Sprite::getTexture()->getSize();
    static const auto numColumns = int(textureSize.x)/int(_originalRect.width);

    n = n % _numFrames;
    _currentFrame = n;

    sf::IntRect newTexRect(_originalRect);
    newTexRect.left += n*_originalRect.width;

    //from hge
    if(newTexRect.left > textureSize.x - _originalRect.width)
    {
        n -= int(textureSize.x - _originalRect.width) / int(_originalRect.width);
        newTexRect.left = _originalRect.width * (n % numColumns);
        newTexRect.top += newTexRect.height * (1 + n / numColumns);
    }

    Sprite::setTextureRect(newTexRect);
}
