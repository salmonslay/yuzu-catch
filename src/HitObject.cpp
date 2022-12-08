#include "HitObject.h"
#include "Constants.h"

namespace yuzu
{
    // x*gScreenWidth/512 will scale the x position from osu!pixels to pixels
    HitObject::HitObject(int x, int w, int h, int t, SDL_Texture *texture) :
            Sprite(static_cast<int>(x * 1.0 * constants::gScreenWidth / 512.0), -100, w, h, texture), time(t) {}

    void HitObject::update()
    {
        Sprite::update();

        // move the hit object down
        SDL_Rect rect = get_rect();
        rect.y += 5 * constants::speedMod;
        set_rect(rect);
    }

} // yuzu