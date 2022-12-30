#ifndef YUZU_CATCH_HIT_OBJECT_H
#define YUZU_CATCH_HIT_OBJECT_H

#include <SDL_render.h>
#include "Sprite.h"

namespace yuzu
{
    enum class HitObjectState {
        /** The hit object is not visible on the screen and has not been summoned yet */
        IDLE,
        /** The hit object is visible on the screen and has been summoned */
        ACTIVE,
        /** The hit object has been hit by the player */
        HIT,
        /** The hit object is on the catcher plate */
        PLATED,
        /** The hit object has been missed by the player, and can no longer be hit */
        MISSED,
        /** The hit object has been queued for removal */
        DESTROYED,
        /** The hit object has been removed from the screen */
        HIDDEN,
    };

    class HitObject : public fruitwork::Sprite {
    public:

        void setState(HitObjectState state) { this->state = state; }

        HitObjectState getState() const { return state; }

        /**
         * The time in milliseconds when the hit object should be hit.
         */
        int time;

        /** Bitfield of the hit sounds. 0=normal, 1=whistle, 2=finish, 3=clap */
        unsigned int hitSounds = 0;

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
        HitObject(int x, int w, int h, int t, SDL_Texture *texture, unsigned int hs = 0);

    private:
        /** The time in milliseconds when the hit object was added as a component, aka summoned. */
        Uint64 startTime;
        static const int START_Y = -100;

        /** The Y point at which the hit object can be hit. */
        static const int HIT_Y = 748;

        /** The Y point at which the hit object is considered missed. */
        static const int MISS_Y = HIT_Y + 8;

        /** How many milliseconds it should take for the hit object to fall from START_Y to HIT_Y. */
        static const int DROP_TIME = 1000;

        HitObjectState state = HitObjectState::IDLE;
    };

} // yuzu

#endif //YUZU_CATCH_HIT_OBJECT_H
