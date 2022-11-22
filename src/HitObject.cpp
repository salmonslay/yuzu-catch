#include "HitObject.h"

namespace yuzu
{

    HitObject::HitObject(int x, int w, int h, int t, SDL_Texture *texture) :
            Sprite(x, 0, w, h)
    {
        this->time = t;
        this->spriteTexture = texture;
    }

} // yuzu