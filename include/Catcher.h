#ifndef YUZU_CATCH_CATCHER_H
#define YUZU_CATCH_CATCHER_H

#include <SDL_render.h>
#include "Sprite.h"
#include "HitObject.h"

namespace yuzu
{

    class Catcher : public fruitwork::Sprite {
    public:
        static Catcher *getInstance(int x, int y, int w, int h);

        void start() override;

        void update() override;

        void onKeyDown(const SDL_Event &e) override;

        void onKeyUp(const SDL_Event &e) override;

        /** (x, x2) is the current range of the plate */
        SDL_Point getPlateRange() const;

        void addFruitToPlate(HitObject *ho);

        void explodePlate();

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
        SDL_FRect preciseRect;
        const float CATCHER_SPEED = 750.0f;


        /** Moves the catcher by the given amount. */
        void moveBy(float x);

        /** Milliseconds between each phantom spawn. */
        const float PHANTOM_INTERVAL = 100.0;
        /** The last time a phantom was spawned. */
        Uint64 lastPhantomSpawn = 0;

        void spawnPhantom();

    };

} // yuzu

#endif //YUZU_CATCH_CATCHER_H
