#ifndef YUZU_CATCH_HITOBJECT_H
#define YUZU_CATCH_HITOBJECT_H

#include <SDL_render.h>
#include "Sprite.h"

namespace yuzu
{
    enum class HitsoundType {
        Normal,
        Whistle,
        Finish,
        Clap
    };

    class HitObject : public fruitwork::Sprite {
    public:
        /**
         * The time in milliseconds when the hit object should be hit.
         */
        int time;

        HitsoundType hitsoundType = HitsoundType::Normal;

        /** Hyper fruits will make the catcher much faster. */
        bool isHyper = false;

        /** Whether or not this hit object is added */
        bool added = false;

        // score stuff

        /** The score granted by the hit object. */
        virtual int getScore() const = 0;

        /** Whether or not the hit object will increase combo. */
        virtual bool addsCombo() const { return true; }

        /** Whether or not the score granted is affected by combo. If this is false, the flat score will be added. */
        virtual bool comboDependent() const { return true; }

        void start() override;

        void update() override;

    protected:
        /**
         * Creates a new hit object.
         * @param x The x position of the hit object in osu!pixels (0-512).
         * @param w The width of the hit object in pixels.
         * @param h The height of the hit object in pixels.
         * @param t The time in milliseconds when the hit object should be hit.
         * @param texture The texture to use for the hit object.
         */
        HitObject(int x, int w, int h, int t, SDL_Texture *texture);

    private:
        /** The time in milliseconds when the hit object was added as a component, aka summoned. */
        Uint64 startTime;
        static const int START_Y = -100;

        /** The Y point at which the hit object can be hit. */
        static const int HIT_Y = 1000;

        /** The Y point at which the hit object is considered missed. */
        static const int MISS_Y = HIT_Y - 20;

        /** How many milliseconds it should take for the hit object to fall from START_Y to HIT_Y. */
        static const int DROP_TIME = 1000;

    };

} // yuzu

#endif //YUZU_CATCH_HITOBJECT_H
