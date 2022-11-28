#ifndef YUZU_CATCH_GAMESCENE_H
#define YUZU_CATCH_GAMESCENE_H

#include <SDL_mixer.h>
#include "Scene.h"

namespace yuzu
{

    class GameScene : public fruitwork::Scene {
    public:
        static GameScene *get_instance();

        bool enter() override;

        bool exit() override;

        void update() override;

        /**
         * Container for the hit object sprites
         */
        struct HitObjectSet {
            SDL_Texture *baseTexture;
            SDL_Texture *overlayTexture;
        };

        /**
         * Container for the hit object samples
         */
        struct HitSampleSet {
            Mix_Chunk *normal;
            Mix_Chunk *whistle;
            Mix_Chunk *finish;
            Mix_Chunk *clap;
        };

    private:
        static GameScene instance;

        // game resources
        std::vector<HitObjectSet> fruitSets;
        HitObjectSet bananaSet;
        SDL_Texture *dropTexture;
        SDL_Texture *backgroundTexture;

        Mix_Music *music;
        HitSampleSet hitSampleSet;

        void initResources();

        GameScene();
    };

} // yuzu

#endif //YUZU_CATCH_GAMESCENE_H