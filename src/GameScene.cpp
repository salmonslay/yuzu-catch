#include <iostream>
#include <SDL_image.h>
#include <chrono>
#include "GameScene.h"
#include "System.h"
#include "ResourceManager.h"
#include "GalleryScene.h"

namespace yuzu
{
    bool GameScene::enter()
    {
        bool success = true;
        std::cout << "GameScene::enter()" << std::endl;

        initResources();

        return success;
    }

    void GameScene::initResources()
    {
        SDL_Log("Loading game resources...");
        auto start = std::chrono::high_resolution_clock::now();

        if (currentBeatmap == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Beatmap is null!");
            return;
        }
        else
        {
            SDL_Log("Loading resources for beatmap \"%s\" (%s)", currentBeatmap->title.c_str(), currentBeatmap->version.c_str());
        }

        // fruits
        std::vector<std::string> fruits = {"apple", "grapes", "orange", "pear"};
        for (std::string &f: fruits)
        {
            HitObjectSet set{};
            set.baseTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("fruit-" + f + ".png").c_str());
            set.overlayTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("fruit-" + f + "-overlay.png").c_str());
            fruitSets.push_back(set);
        }

        // banana
        bananaSet.baseTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), "fruit-bananas.png");
        bananaSet.overlayTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), "fruit-bananas-overlay.png");

        // drop
        dropTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), "fruit-drop.png");

        // samples
        std::string sample = "normal";
        if (currentBeatmap->sampleType == Beatmap::SampleType::SOFT)
            sample = "soft";
        else if (currentBeatmap->sampleType == Beatmap::SampleType::DRUM)
            sample = "drum";

        hitSampleSet.normal = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/" + sample + "-hitnormal.wav").c_str());
        hitSampleSet.whistle = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/" + sample + "-hitwhistle.wav").c_str());
        hitSampleSet.finish = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/" + sample + "-hitfinish.wav").c_str());
        hitSampleSet.clap = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/" + sample + "-hitclap.wav").c_str());

        music = currentBeatmap->getAudio();

        // @see https://stackoverflow.com/a/12883734/11420970
        auto finish = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        SDL_Log("Game resources loaded in %lldms", ms.count());

        backgroundOverlay = fruitwork::Rectangle::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, {0, 0, 0, 128});
        backgroundSprite = fruitwork::CoveringSprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, currentBeatmap->getBackgroundTexture());

        add_component(backgroundSprite);
        add_component(backgroundOverlay);
    }

    bool GameScene::exit()
    {
        bool success = true;
        std::cout << "Exiting GameScene" << std::endl;

        for (auto &c: components)
        {
            delete c;
        }

        for (HitObjectSet &s: fruitSets)
        {
            SDL_DestroyTexture(s.baseTexture);
            SDL_DestroyTexture(s.overlayTexture);
        }

        SDL_DestroyTexture(bananaSet.baseTexture);
        SDL_DestroyTexture(bananaSet.overlayTexture);
        SDL_DestroyTexture(dropTexture);

        Mix_FreeChunk(hitSampleSet.normal);
        Mix_FreeChunk(hitSampleSet.whistle);
        Mix_FreeChunk(hitSampleSet.finish);
        Mix_FreeChunk(hitSampleSet.clap);

        return success;
    }

    GameScene::GameScene()
    =
    default;

    GameScene *GameScene::get_instance()
    {
        return &instance;
    }

    void GameScene::update()
    {

    }

    GameScene GameScene::instance;

} // yuzu