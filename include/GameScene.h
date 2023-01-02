#ifndef YUZU_CATCH_GAME_SCENE_H
#define YUZU_CATCH_GAME_SCENE_H

#include <SDL_mixer.h>
#include "Scene.h"
#include "ResponsiveSprite.h"
#include "CoveringSprite.h"
#include "Rectangle.h"
#include "Catcher.h"
#include "Label.h"
#include "Score.h"
#include "ConfettiCannon.h"
#include "ScoreDisplay.h"

namespace yuzu
{

    class GameScene : public fruitwork::Scene {
    public:
        static GameScene *getInstance();

        bool enter() override;

        bool exit() override;

        void update() override;

        Catcher *getCatcher() const { return catcher; }

        bool isKiai() const { return kiai; }

        void processFruit(HitObject *ho);

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

        fruitwork::CoveringSprite *backgroundSprite = nullptr;
        /** Background overlay is a black transparent rectangle making the background darker */
        fruitwork::Rectangle *backgroundOverlay = nullptr;

        Mix_Music *music;
        HitSampleSet hitSampleSet;
        Mix_Chunk *comboBreakSample;
        Mix_Chunk *bananaSample;

        // game objects
        Catcher *catcher = nullptr;
        Score *score = nullptr;
        ScoreDisplay *scoreDisplay = nullptr;

        Uint64 startTime = -1;

        // UI
        fruitwork::Label *scoreLabel = nullptr;
        fruitwork::Label *comboLabel = nullptr;
        fruitwork::Label *accuracyLabel = nullptr;
        const SDL_Color TEXT_NORMAL_COLOR = {207, 249, 250, 255};
        const SDL_Color TEXT_KIAI_COLOR = {219, 207, 250, 255};

        void initResources();

        void startGame();

        GameScene();

        bool kiai = false;

        void setKiai(bool kiai);

        fruitwork::ConfettiCannon *cannon = nullptr;
    };

} // yuzu

#endif //YUZU_CATCH_GAME_SCENE_H