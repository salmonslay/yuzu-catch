#ifndef YUZU_CATCH_FRUIT_H
#define YUZU_CATCH_FRUIT_H

#include "Sprite.h"
#include "HitObject.h"

namespace yuzu
{
    /**
     * A fruit is a hit object that can be caught by the player. It gives 300 score based on combo.
     * Fruits are constructed by two sprites, one base and one overlay.
     */
    class Fruit : public HitObject {
    public:
        static Fruit *getInstance(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color);

        int getScore() const override { return 300; }

        void draw() const override;

        ~Fruit() override;

    protected:
        Fruit(int x, int w, int h, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color);

    private:
        fruitwork::Sprite *overlay = nullptr;
    };

} // yuzu

#endif //YUZU_CATCH_FRUIT_H
