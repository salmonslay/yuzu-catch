#include "HitObject.h"
#include "Constants.h"
#include "GameScene.h"

namespace yuzu
{
    // x*(gScreenWidth-100)/512+50 will scale the x position from osu!pixels to pixels, centered
    HitObject::HitObject(int x, int w, int h, int t, SDL_Texture *texture, unsigned int hs) :
            Sprite(static_cast<int>(x * (constants::gScreenWidth - 100.0) / 512.0 + 50), START_Y, w, h, texture),
            time(t), hitSounds(hs) {}

    void HitObject::start()
    {
        startTime = SDL_GetTicks64();
        state = HitObjectState::ACTIVE;
    }

    void HitObject::update()
    {
        Component::update();

        if (state == HitObjectState::PLATED)
            return; // don't move if plated, control is given to the catcher

        auto gameScene = GameScene::getInstance();

        // move the hit object down
        SDL_Rect rect = getRect();
        // the speed is calculated from DROP_TIME, which is the milliseconds it takes to fall from START_Y to HIT_Y
        rect.y = static_cast<int>((START_Y - rect.h) + (HIT_Y - START_Y) * (SDL_GetTicks64() - startTime) / DROP_TIME);
        setRect(rect);

        // check if the hit object is out of hit bounds
        if ((rect.y + rect.h) > MISS_Y && state == HitObjectState::ACTIVE)
        {
            state = HitObjectState::MISSED;
            gameScene->processFruit(this);
        }

            // check if the hit object is in hit bounds
            // since rhythm games are very precise, we can not use collision detection here
        else if ((rect.y + rect.h) > HIT_Y && state == HitObjectState::ACTIVE)
        {
            SDL_Point plateRange = GameScene::getInstance()->getCatcher()->getPlateRange(); // (x, x2)

            // the fruit only needs to be partially in the plate range to be hit, not fully
            if (rect.x + rect.w > plateRange.x && rect.x < plateRange.y)
            {
                // SDL_Log("HIT, summoned %llu ms ago", SDL_GetTicks64() - startTime);
                state = HitObjectState::HIT;
                gameScene->processFruit(this);
            }
        }

            // remove & hide the hit object if it is out of bounds
        else if (rect.y > constants::gScreenHeight)
        {
            state = HitObjectState::HIDDEN;
        }
    }

} // yuzu