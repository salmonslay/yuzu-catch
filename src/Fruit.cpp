#include "Fruit.h"

namespace yuzu
{

    Fruit *Fruit::getInstance(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
    {
        return new Fruit(x, w, h, t, texture, overlayTexture, color);
    }

    Fruit::Fruit(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
            : HitObject(x, w, h, t, texture)
    {
        overlay = fruitwork::Sprite::getInstance(x, 0, w, h, overlayTexture);
        overlay->setColorMod(color);
    }

    void Fruit::draw() const
    {
        overlay->set_rect(this->get_rect());
        Sprite::draw();
        overlay->draw();
    }

    Fruit::~Fruit()
    {
        delete overlay;
    }

} // yuzu