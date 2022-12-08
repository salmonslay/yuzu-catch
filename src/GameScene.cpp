#include <iostream>
#include <SDL_image.h>
#include <chrono>
#include <thread>
#include "GameScene.h"
#include "System.h"
#include "ResourceManager.h"
#include "GalleryScene.h"
#include "future"

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

        backgroundOverlay = fruitwork::Rectangle::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, {0, 0, 0, 128});
        backgroundSprite = fruitwork::CoveringSprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, currentBeatmap->getBackgroundTexture());

        int catcherWidth = 610 / 2;
        int catcherHeight = 648 / 2;
        catcher = Catcher::getInstance((constants::gScreenWidth - catcherWidth) / 2, 730, catcherWidth, catcherHeight);

        // ui
        scoreLabel = fruitwork::Label::getInstance(405, 0, 390, 98, "0000000");
        scoreLabel->setFontSize(72);
        scoreLabel->setColor({207, 249, 250, 255});
        scoreLabel->setAlignment(fruitwork::Label::Alignment::CENTER);

        comboLabel = fruitwork::Label::getInstance(795, 36, 157, 55, "x0");
        comboLabel->setFontSize(40);
        comboLabel->setColor({207, 249, 250, 255});
        comboLabel->setAlignment(fruitwork::Label::Alignment::LEFT);

        accuracyLabel = fruitwork::Label::getInstance(248, 36, 157, 55, "100%");
        accuracyLabel->setFontSize(40);
        accuracyLabel->setColor({207, 249, 250, 255});
        accuracyLabel->setAlignment(fruitwork::Label::Alignment::RIGHT);

        currentBeatmap->loadGameplayInformation(fruitSets, bananaSet, dropTexture, hitSampleSet);

        // @see https://stackoverflow.com/a/12883734/11420970
        auto finish = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        SDL_Log("Game resources loaded in %lldms", ms.count());

        add_component(backgroundSprite);
        add_component(backgroundOverlay);
        add_component(catcher);
        add_component(scoreLabel);
        add_component(comboLabel);
        add_component(accuracyLabel);

        startGame();
    }

    void GameScene::startGame()
    {
        startTime = SDL_GetTicks64();

        // play music after 800 ms
        std::thread([this]()
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(800));
                        Mix_PlayMusic(music, 0);
                    }).detach();
    }

    void GameScene::update()
    {
        // add each hit object to the scene if it's time
        if (startTime != -1)
        {
            uint64_t currentTime = SDL_GetTicks64() - startTime;
            for (HitObject *hitObject: currentBeatmap->hitObjects)
            {
                if (hitObject->time <= currentTime && !hitObject->added)
                {
                    add_component(hitObject);
                    hitObject->added = true;
                }
            }
        }
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

    GameScene GameScene::instance;

} // yuzu