#include "JuiceDrop.h"

namespace yuzu
{
    JuiceDrop *JuiceDrop::getInstance(int x, int t, SDL_Texture *texture, SDL_Color color)
    {
        return new JuiceDrop(x, t, texture, color);
    }

    JuiceDrop::JuiceDrop(int x, int t, SDL_Texture *texture, SDL_Color color)
            : HitObject(x, WIDTH, HEIGHT, t, texture)
    {
        this->setColorMod(color);
    }

    void JuiceDrop::draw() const
    {
        Sprite::draw();
    }

} // yuzu