#include <SDL_image.h>
#include "DifficultyButton.h"
#include "Beatmap.h"
#include "ResourceManager.h"
#include "GalleryScene.h"
#include "GameScene.h"

namespace yuzu
{
    DifficultyButton *DifficultyButton::getInstance(int x, int y, int w, int h, Beatmap *beatmap)
    {
        return new DifficultyButton(x, y, w, h, beatmap);
    }

    DifficultyButton::DifficultyButton(int x, int y, int w, int h, Beatmap *beatmap)
            : fruitwork::Button(x, y, w, h, ""), beatmap(beatmap)
    {
        switch (beatmap->difficulty)
        {
            case Beatmap::Difficulty::CUP:
                difficultyName = "Cup";
                starCount = 1;
                starModColor = {79, 192, 255, 255};
                break;
            case Beatmap::Difficulty::SALAD:
                difficultyName = "Salad";
                starCount = 2;
                starModColor = {124, 255, 79, 255};
                break;
            case Beatmap::Difficulty::PLATTER:
                difficultyName = "Platter";
                starCount = 3;
                starModColor = {246, 240, 92, 255};
                break;
            case Beatmap::Difficulty::RAIN:
                difficultyName = "Rain";
                starCount = 4;
                starModColor = {255, 78, 111, 255};
                break;
            case Beatmap::Difficulty::OVERDOSE:
                difficultyName = "Overdose";
                starCount = 5;
                starModColor = {198, 69, 184, 255};
                break;
            case Beatmap::Difficulty::FEAST:
                difficultyName = "Feast";
                starCount = 5;
                starModColor = {101, 99, 222, 255};
                break;
        }

        if (starTexture == nullptr)
            starTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("star.png").c_str());

        for (int i = 0; i < 5; i++)
        {
            stars.push_back(fruitwork::Sprite::getInstance(x + 192 + (i * 52), y + 12, 52, 50, starTexture));
            stars[i]->setColorMod(starModColor);
            if (i >= starCount)
                stars[i]->setAlphaMod(100);
        }

        difficultyLabel = fruitwork::Label::getInstance(x + 10, y + 11, 165, 48, difficultyName);
        difficultyLabel->setFontSize(36);
        difficultyLabel->setAlignment(fruitwork::Label::Alignment::CENTER);
        difficultyLabel->setColor({72, 72, 72, 255});

        registerCallback([](fruitwork::Button *src)
                         {
                             auto *button = (DifficultyButton *) src;
                             currentBeatmap = button->beatmap;
                             SDL_Log("Starting beatmap \"%s\" (%s)...", currentBeatmap->title.c_str(), currentBeatmap->version.c_str());
                             fruitwork::sys.setNextScene(GameScene::get_instance());
                         });
    }

    void DifficultyButton::draw() const
    {
        Button::draw();
        difficultyLabel->draw();
        for (auto &star: stars)
            star->draw();
    }

    DifficultyButton::~DifficultyButton()
    {
        delete difficultyLabel;
        for (auto &star: stars)
            delete star;
    }

} // yuzu