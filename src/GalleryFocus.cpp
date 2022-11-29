#include <algorithm>
#include "GalleryFocus.h"
#include "ResourceManager.h"
#include "DifficultyButton.h"
#include "GalleryScene.h"

namespace yuzu
{
    GalleryFocus *yuzu::GalleryFocus::getInstance(int x, int y)
    {
        return new GalleryFocus(x, y);
    }

    GalleryFocus::GalleryFocus(int x, int y) :
            Component(x, y, 530, 818)
    {
        background = fruitwork::ResponsiveSprite::getInstance(x, y, 528, 297, fruitwork::ResourceManager::getTexturePath("placeholder.png"));
        coverBackdropRect = {x, y, 528, 297};

        songTitle = fruitwork::Label::getInstance(x, y + 300, 528, 63, "Pick a song to the left!");
        songTitle->setFontSize(36);
        songTitle->setColor({255, 255, 255, 255});

        songArtist = fruitwork::Label::getInstance(x, y + 353, 528, 46, "");
        songArtist->setFontSize(32);
        songArtist->setColor({255, 255, 255, 178});

        songCreator = fruitwork::Label::getInstance(x, y + 403, 528, 36, "");
        songCreator->setFontSize(16);
        songCreator->setColor({255, 255, 255, 178});

        int songX = x + 420;
        int songY = y + 257;

        songTime = fruitwork::Label::getInstance(songX, songY, 100, 36, "00:00");
        songTime->setFontSize(36);
        songTime->setColor({255, 255, 255, 255});

        lengthBackdropRect = {songX - 2, songY, 106, 36};
    }

    void GalleryFocus::start()
    {
        background->start();
    }

    void GalleryFocus::update()
    {
        for (auto &difficultyButton: difficultyButtons)
            difficultyButton->update();
    }

    void GalleryFocus::draw() const
    {
        // black square behind the sprite
        SDL_SetRenderDrawColor(fruitwork::sys.get_renderer(), 0, 0, 0, 0);
        SDL_RenderFillRect(fruitwork::sys.get_renderer(), &coverBackdropRect);


        background->draw();
        songTitle->draw();
        songArtist->draw();
        songCreator->draw();

        // gray transparent square behind the song time
        SDL_SetRenderDrawColor(fruitwork::sys.get_renderer(), 0, 0, 0, 178);
        SDL_SetRenderDrawBlendMode(fruitwork::sys.get_renderer(), SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(fruitwork::sys.get_renderer(), &lengthBackdropRect);
        SDL_SetRenderDrawColor(fruitwork::sys.get_renderer(), 255, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(fruitwork::sys.get_renderer(), SDL_BLENDMODE_NONE);

        songTime->draw();

        for (auto &difficultyButton: difficultyButtons)
            difficultyButton->draw();
    }

    void GalleryFocus::onMouseDown(const SDL_Event &e)
    {
        for (auto &difficultyButton: difficultyButtons)
            difficultyButton->onMouseDown(e);
    }

    void GalleryFocus::onMouseUp(const SDL_Event &e)
    {
        for (auto &difficultyButton: difficultyButtons)
            difficultyButton->onMouseUp(e);
    }

    void GalleryFocus::setBeatmap(std::vector<Beatmap *> beatmapSet)
    {
        // TODO: clear old difficulty buttons

        Beatmap *beatmap = beatmapSet[0];
        background->setTexture(beatmap->getBackgroundTexture());
        songTitle->setText(beatmap->title);
        songArtist->setText(beatmap->artist);
        songCreator->setText("Mapped by " + beatmap->creator);
        //songTime->setText(beatmap->time);

        // sort beatmaps by difficulty, this could ideally be done elsewhere but i think it's fine here
        std::sort(beatmapSet.begin(), beatmapSet.end(), [](Beatmap *a, Beatmap *b) {
            return a->difficulty < b->difficulty;
        });

        for (int i = 0; i < std::min(static_cast<int>(beatmapSet.size()), 4); i++)
            difficultyButtons.push_back(DifficultyButton::getInstance(get_rect().x, get_rect().y + 460 + (90 * i), 460, 76, beatmapSet[i]));
    }

    GalleryFocus::~GalleryFocus()
    {
        delete background;
        delete songTitle;
        delete songArtist;
        delete songCreator;
        delete songTime;

        for (auto &difficultyButton: difficultyButtons)
            delete difficultyButton;
    }
} // yuzu