#include "Banana.h"

namespace yuzu
{
    Banana *Banana::getInstance(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
    {
        return new Banana(x, w, h, t, texture, overlayTexture, color);
    }

    Banana::Banana(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
            : Fruit(x, w, h, t, texture, overlayTexture, color) {}
} // yuzu