#ifndef YUZU_CATCH_BANANA_H
#define YUZU_CATCH_BANANA_H

#include "Fruit.h"

namespace yuzu
{

    /**
     * Bananas are fruits that give a 1000 flat score, independent of combo.
     * They're only spawned during banana showers.
     */
    class Banana : public Fruit {
    public:
        static Banana *getInstance(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color);

        int getScore() const override { return 1000; }

        bool addsCombo() const override { return false; }

        bool comboDependent() const override { return false; }

    protected:
        Banana(int x, int t, SDL_Texture *texture, SDL_Texture *overlayTexture, SDL_Color color);
    };

} // yuzu

#endif //YUZU_CATCH_BANANA_H
