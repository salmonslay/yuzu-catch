#include <iostream>
#include <SDL_image.h>
#include <chrono>
#include <thread>
#include "GameScene.h"
#include "System.h"
#include "ResourceManager.h"
#include "GalleryScene.h"
#include "JuiceDrop.h"
#include "Banana.h"
#include "Main.h"

namespace yuzu
{
    bool GameScene::enter()
    {
        bool success = true;
        SDL_Log("Entering GameScene...");

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
            set.baseTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-" + f + ".png").c_str());
            set.overlayTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-" + f + "-overlay.png").c_str());
            fruitSets.push_back(set);
        }

        // banana
        bananaSet.baseTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-bananas.png").c_str());
        bananaSet.overlayTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-bananas-overlay.png").c_str());

        // drop
        dropTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-drop.png").c_str());

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
        comboBreakSample = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/combobreak.wav").c_str());
        bananaSample = Mix_LoadWAV(fruitwork::ResourceManager::getAudioPath("hs/catch-banana.wav").c_str());
        music = currentBeatmap->getAudio();

        backgroundOverlay = fruitwork::Rectangle::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, {0, 0, 0, 128});
        backgroundSprite = fruitwork::CoveringSprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, currentBeatmap->getBackgroundTexture());

        int catcherWidth = 610 / 2;
        int catcherHeight = 648 / 2;
        catcher = Catcher::getInstance((constants::gScreenWidth - catcherWidth) / 2, 734, catcherWidth, catcherHeight);

        score = Score::getInstance();

        // ui
        scoreLabel = fruitwork::Label::getInstance(793, 0, 390, 98, "0000000");
        scoreLabel->setFontSize(72);
        scoreLabel->setAlignment(fruitwork::Label::Alignment::RIGHT);
        scoreLabel->setColor(TEXT_NORMAL_COLOR);

        comboLabel = fruitwork::Label::getInstance(17, 845, 157, 55, "x0");
        comboLabel->setFontSize(40);
        comboLabel->setAlignment(fruitwork::Label::Alignment::LEFT);
        comboLabel->setColor(TEXT_NORMAL_COLOR);

        accuracyLabel = fruitwork::Label::getInstance(943, 98, 240, 55, "100%");
        accuracyLabel->setFontSize(40);
        accuracyLabel->setAlignment(fruitwork::Label::Alignment::RIGHT);
        accuracyLabel->setColor(TEXT_NORMAL_COLOR);

        progressBar = fruitwork::Rectangle::getInstance(0, constants::gScreenHeight - PROGRESS_BAR_HEIGHT,
                                                        constants::gScreenWidth, PROGRESS_BAR_HEIGHT,
                                                        TEXT_NORMAL_COLOR);

        progressBarFill = fruitwork::Rectangle::getInstance(0, constants::gScreenHeight - PROGRESS_BAR_HEIGHT,
                                                            0, PROGRESS_BAR_HEIGHT,
                                                            {0, 0, 0, 128});

        currentBeatmap->loadGameplayInformation(fruitSets, bananaSet, dropTexture, hitSampleSet);

        cannon = fruitwork::ConfettiCannon::getInstance(1240, 900, 24, 24, fruitwork::ResourceManager::getTexturePath("star.png"));
        cannon->setColors(currentBeatmap->comboColours);

        // @see https://stackoverflow.com/a/12883734/11420970
        auto finish = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        SDL_Log("Game resources loaded in %lldms", ms.count());

        addComponent(backgroundSprite, -100);
        addComponent(backgroundOverlay, -99);
        addComponent(progressBar, -98);
        addComponent(progressBarFill, -97);
        addComponent(catcher, 0);
        addComponent(scoreLabel, 1);
        addComponent(comboLabel, 0);
        addComponent(accuracyLabel, 0);
        addComponent(cannon, 0);
        addComponent(score, 0);

        yuzu::ses.registerKeyboardEvent(SDLK_ESCAPE, [this]()
        {
            fruitwork::sys.setNextScene(GalleryScene::getInstance());
        });

        startGame();
    }

    void GameScene::startGame()
    {
        startTime = SDL_GetTicks64();

        std::thread([this]()
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
                    int z = dynamic_cast<JuiceDrop *>(hitObject) != nullptr ? -50 : -49; // drops are behind fruits
                    addComponent(hitObject, z);
                    hitObject->added = true;
                }
            }

            // go through all timing points
            bool k = false;
            for (Beatmap::TimingPoint timingPoint: currentBeatmap->timingPoints)
            {
                if (timingPoint.offset <= currentTime - 1000)
                    k = timingPoint.isKiai;
            }
            setKiai(k);

            if (scoreDisplay == nullptr)
            {
                if (currentBeatmap->length + 1500 <= currentTime)
                {
                    SDL_Log("Game finished, displaying score.");

                    scoreDisplay = ScoreDisplay::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, score);
                    addComponent(scoreDisplay, 100);
                }
            }

            // update progress bar
            SDL_Rect rect = progressBarFill->getRect();
            rect.w = (int) (constants::gScreenWidth * (currentTime / (currentBeatmap->length + 1500.0)));
            progressBarFill->setRect(rect);
        }

        // update score
        std::string scoreText = std::to_string(score->getDisplayScore());
        scoreText.insert(0, 7 - scoreText.length(), '0'); // pad with zeroes
        scoreLabel->setText(scoreText);

        std::string comboText = "x" + std::to_string(score->getCombo());
        comboLabel->setText(comboText);

        accuracyLabel->setText(score->getAccuracyString());
    }

    bool GameScene::exit()
    {
        bool success = true;
        SDL_Log("Exiting GameScene...");

        for (auto &c: components)
        {
            if (dynamic_cast<HitObject *>(c) == nullptr) // hit objects are deleted by the beatmap
                delete c;
        }
        components.clear(); // clear after removing to avoid invalid memory access

        for (HitObjectSet &s: fruitSets)
        {
            SDL_DestroyTexture(s.baseTexture);
            SDL_DestroyTexture(s.overlayTexture);
        }
        fruitSets.clear();

        SDL_DestroyTexture(bananaSet.baseTexture);
        SDL_DestroyTexture(bananaSet.overlayTexture);
        SDL_DestroyTexture(dropTexture);

        Mix_FreeChunk(hitSampleSet.normal);
        Mix_FreeChunk(hitSampleSet.whistle);
        Mix_FreeChunk(hitSampleSet.finish);
        Mix_FreeChunk(hitSampleSet.clap);

        Mix_FreeChunk(comboBreakSample);
        Mix_FreeChunk(bananaSample);

        yuzu::ses.deregisterKeyboardEvent(SDLK_ESCAPE);


        for (auto &ho: currentBeatmap->hitObjects)
            delete ho;
        currentBeatmap->hitObjects.clear();

        Mix_HaltMusic();

        return success;
    }

    GameScene::GameScene()
    =
    default;

    GameScene *GameScene::getInstance()
    {
        return &instance;
    }

    void GameScene::processFruit(HitObject *ho)
    {
        Fruit *f = dynamic_cast<Fruit *>(ho);
        if (ho->getState() == HitObjectState::HIT)
        {
            if (dynamic_cast<Banana *>(ho) != nullptr)
            {
                Mix_PlayChannel(-1, bananaSample, 0);
            }
            else if (f != nullptr)
            {
                if (ho->hitSounds & 1)
                    Mix_PlayChannel(-1, hitSampleSet.whistle, 0);
                if (ho->hitSounds & 2)
                    Mix_PlayChannel(-1, hitSampleSet.finish, 0);
                if (ho->hitSounds & 4)
                    Mix_PlayChannel(-1, hitSampleSet.clap, 0);

                Mix_PlayChannel(-1, hitSampleSet.normal, 0);
            }

            if (dynamic_cast<Fruit *>(ho) != nullptr)
            {
                catcher->addFruitToPlate(ho);
            }
            else
            {
                ho->setState(HitObjectState::HIDDEN);
            }
        }
        else
        {
            if (score->getCombo() > 15 && ho->addsCombo())
                Mix_PlayChannel(-1, comboBreakSample, 0);
        }

        if (f != nullptr && f->isLastInCombo()) // clear the plate if last combo
            catcher->explodePlate();

        score->processHitObject(ho, isKiai());
    }

    void GameScene::setKiai(bool k)
    {
        if (k == kiai)
            return; // only process if changed

        kiai = k;

        if (k)
        {
            SDL_Log("Kiai time!");
            cannon->setRect({1200, 900, 24, 24});
            cannon->fire(225, 30, 150, 1000, 3000);

            cannon->setRect({0, 900, 24, 24});
            cannon->fire(315, 30, 150, 1000, 3000);
        }
        else
        {
            SDL_Log("Kiai ended.");
        }

        scoreLabel->setColor(kiai ? TEXT_KIAI_COLOR : TEXT_NORMAL_COLOR);
        comboLabel->setColor(kiai ? TEXT_KIAI_COLOR : TEXT_NORMAL_COLOR);
        accuracyLabel->setColor(kiai ? TEXT_KIAI_COLOR : TEXT_NORMAL_COLOR);
    }


    GameScene GameScene::instance;

} // yuzu