#ifndef YUZU_CATCH_CATCHER_H
#define YUZU_CATCH_CATCHER_H

#include <SDL_render.h>
#include "Sprite.h"

namespace yuzu
{

    class Catcher : public fruitwork::Sprite {
    public:
        static Catcher *getInstance(int x, int y, int w, int h);

        void start() override;

        void update() override;

        void onKeyDown(const SDL_Event &e) override;

        void onKeyUp(const SDL_Event &e) override;

        /** Moves the catcher by the given amount. */
        void moveBy(int x);

        ~Catcher() override;

    protected:

    private:
        Catcher(int x, int y, int w, int h);

        bool isDash = false;
        bool moveRight = false;
        bool moveLeft = false;

        SDL_Texture *textureIdle;
        SDL_Texture *textureKiai;
        SDL_Texture *textureFail;

        const int catcherSpeed = 7;
    };

} // yuzu

#endif //YUZU_CATCH_CATCHER_H
