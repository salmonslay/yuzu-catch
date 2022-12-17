#include "Banana.h"

namespace yuzu
{
    Banana *Banana::getInstance(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
    {
        return new Banana(x, t, texture, overlayTexture, color);
    }

    Banana::Banana(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
            : Fruit(x, t, texture, overlayTexture, color, 0) {}
} // yuzu