#include "HitObject.h"
#include "Constants.h"
#include "GameScene.h"

namespace yuzu
{
    // x*(gScreenWidth-100)/512+50 will scale the x position from osu!pixels to pixels, centered
    HitObject::HitObject(int x, int w, int h, int t, SDL_Texture *texture) :
            Sprite(static_cast<int>(x * (constants::gScreenWidth - 100.0) / 512.0 + 50), START_Y, w, h, texture), time(t) {}

    void HitObject::start()
    {
        startTime = SDL_GetTicks64();
        state = HitObjectState::ACTIVE;
    }

    void HitObject::update()
    {
        Sprite::update();

        // move the hit object down
        SDL_Rect rect = getRect();
        // the speed is calculated from DROP_TIME, which is the milliseconds it takes to fall from START_Y to HIT_Y
        rect.y = static_cast<int>(START_Y + (HIT_Y - START_Y) * (SDL_GetTicks64() - startTime) / DROP_TIME);
        setRect(rect);

        // check if the hit object is out of hit bounds
        if (rect.y > MISS_Y && state == HitObjectState::ACTIVE)
            state = HitObjectState::MISSED;

        // remove & destroy if out of the screen
        if (rect.y > constants::gScreenHeight)
        {
            state = HitObjectState::DESTROYED;
            yuzu::GameScene::getInstance()->removeComponent(this, true);
        }
    }

} // yuzu