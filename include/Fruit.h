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
                                  SDL_Color color, unsigned int hs);

        int getScore() const override { return 300; }

        bool isLastInCombo() const { return lastInCombo; }

        void setLastInCombo(bool l) { this->lastInCombo = l; }

        void setFading() { this->fading = true; }

        void draw() const override;

        void update() override;

        ~Fruit() override;

    protected:
        Fruit(int x, int t,
              SDL_Texture *texture, SDL_Texture *overlayTexture,
              SDL_Color color, unsigned int hs);

    private:
        fruitwork::Sprite *overlay = nullptr;

        static const int WIDTH = 128 / 1.5;
        static const int HEIGHT = 128 / 1.5;

        /** Whether or not this object is the last object in a combo */
        bool lastInCombo = false;

        float a = 255.0f;
        bool fading = false;
    };

} // yuzu

#endif //YUZU_CATCH_FRUIT_H
