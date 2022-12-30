#ifndef YUZU_CATCH_FRUIT_H
#define YUZU_CATCH_FRUIT_H

#include "Sprite.h"
#include "HitObject.h"

namespace yuzu
{
    /**
     * A fruit is a hit object that can be caught by the player. It gives 300 score based on combo.
     * Fruits are constructed by two sprites, one base and one overlay, where the base has a color mod applied to it.
     */
    class Fruit : public HitObject {
    public:
        static Fruit *getInstance(int x, int t,
                                  SDL_Texture *texture, SDL_Texture *overlayTexture,
                                  SDL_Color color, unsigned int hs, bool newCombo = false);

        int getScore() const override { return 300; }

        bool isNewCombo() const { return newCombo; }

        void draw() const override;

        ~Fruit() override;

    protected:
        Fruit(int x, int t,
              SDL_Texture *texture, SDL_Texture *overlayTexture,
              SDL_Color color, unsigned int hs, bool newCombo = false);

    private:
        fruitwork::Sprite *overlay = nullptr;

        static const int WIDTH = 128 / 1.5;
        static const int HEIGHT = 128 / 1.5;

        /** Whether or not this object is a new combo */
        bool newCombo;
    };

} // yuzu

#endif //YUZU_CATCH_FRUIT_H
