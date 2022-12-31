#include "Fruit.h"
#include "Constants.h"

namespace yuzu
{

    Fruit *Fruit::getInstance(int x, int t,
                              SDL_Texture *texture, SDL_Texture *overlayTexture,
                              SDL_Color color, unsigned int hs)
    {
        return new Fruit(x, t, texture, overlayTexture, color, hs);
    }

    Fruit::Fruit(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color, unsigned int hs)
            : HitObject(x, WIDTH, HEIGHT, t, texture, hs)
    {
        overlay = fruitwork::Sprite::getInstance(x, 0, WIDTH, HEIGHT, overlayTexture);
        setColorMod(color);
    }

    void Fruit::update()
    {
        HitObject::update();

        setAlphaMod(static_cast<int>(a));
        overlay->setAlphaMod(static_cast<int>(a));

        if (a <= 0.0f)
        {
            fading = false;
            a = 0;
            setState(HitObjectState::HIDDEN);
            delete getPhysicsBody();
            setPhysicsBody(nullptr);
        }

        if (fading)
        {
            a -= constants::speedMod * 1.5;
        }
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