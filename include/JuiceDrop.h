#ifndef YUZU_CATCH_JUICEDROP_H
#define YUZU_CATCH_JUICEDROP_H

#include "HitObject.h"

namespace yuzu
{

    /**
     * Juice drops are tiny fruits that gives 50 score, based on combo.
     */
    class JuiceDrop : public HitObject {
    public:
        static JuiceDrop *getInstance(int x, int w, int h, int t, SDL_Texture *texture, SDL_Color color);

        int getScore() const override { return 50; }

        bool addsCombo() const override { return false; }

    protected:
        JuiceDrop(int x, int w, int h, int t, SDL_Texture *texture, SDL_Color color);

    };

} // yuzu

#endif //YUZU_CATCH_JUICEDROP_H
