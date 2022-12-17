#include "Fruit.h"

namespace yuzu
{

    Fruit *Fruit::getInstance(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
    {
        return new Fruit(x, t, texture, overlayTexture, color);
    }

    Fruit::Fruit(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color)
            : HitObject(x, WIDTH, HEIGHT, t, texture)
    {
        overlay = fruitwork::Sprite::getInstance(x, 0, WIDTH, HEIGHT, overlayTexture);
        setColorMod(color);
    }

    void Fruit::draw() const
    {
        if (getState() == HitObjectState::HIDDEN)
            return;

        overlay->setRect(this->getRect());
        Sprite::draw();
        overlay->draw();
    }

    Fruit::~Fruit()
    {
        delete overlay;
    }

} // yuzu