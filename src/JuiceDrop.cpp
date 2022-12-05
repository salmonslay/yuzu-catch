#include "JuiceDrop.h"

namespace yuzu
{
    JuiceDrop *JuiceDrop::getInstance(int x, int w, int h, int t, SDL_Texture *texture, SDL_Color color)
    {
        return new JuiceDrop(x, w, h, t, texture, color);
    }

    JuiceDrop::JuiceDrop(int x, int w, int h, int t, SDL_Texture *texture, SDL_Color color)
            : HitObject(x, w, h, t, texture)
    {
        this->setColorMod(color);
    }

} // yuzu