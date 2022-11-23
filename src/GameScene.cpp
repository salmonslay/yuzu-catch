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
        SDL_DestroyTexture(backgroundTexture);

        Mix_FreeChunk(hitSampleSet.normal);
        Mix_FreeChunk(hitSampleSet.whistle);
        Mix_FreeChunk(hitSampleSet.finish);
        Mix_FreeChunk(hitSampleSet.clap);

        return success;
    }

    void GameScene::initResources()
    {
        SDL_Log("Loading game resources...");
        auto start = std::chrono::high_resolution_clock::now();

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

        // background
        backgroundTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("jerafina.png").c_str());

        // samples
        // TODO: load correct sample type from beatmap, for now we just use soft
        hitSampleSet.normal = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/soft-hitnormal.wav").c_str());
        hitSampleSet.whistle = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/soft-hitwhistle.wav").c_str());
        hitSampleSet.finish = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/soft-hitfinish.wav").c_str());
        hitSampleSet.clap = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/soft-hitclap.wav").c_str());

        // https://stackoverflow.com/a/12883734/11420970
        auto finish = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        SDL_Log("Game resources loaded in %lldms", ms.count());
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