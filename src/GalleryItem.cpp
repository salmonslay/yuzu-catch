#include "GalleryItem.h"
#include "System.h"
#include "GalleryScene.h"

namespace yuzu
{
    GalleryItem *GalleryItem::getInstance(Beatmap *beatmap, int x, int y, int beatmapIndex)
    {
        return new GalleryItem(beatmap, x, y, beatmapIndex);
    }

    GalleryItem::GalleryItem(Beatmap *beatmap, int x, int y, int beatmapIndex)
            : Component(x, y, 246, 199), beatmapIndex(beatmapIndex)
    {
        sprite = fruitwork::ImageButton::getInstance(x, y, 246, 139, beatmap->getBackgroundTexture());
        sprite->registerCallback([this](fruitwork::Button *src)
                                 {
                                     yuzu::GalleryScene::get_instance()->setSelectedBeatmap(this->beatmapIndex); // beatmapIndex instead of 1 - how?
                                 });

        coverBackdrop = fruitwork::Rectangle::getInstance(x, y, 246, 139, {0, 0, 0, 255});
        coverFrame = fruitwork::Rectangle::getInstance(x - 1, y - 1, 246 + 2, 139 + 2, {255, 255, 255, 255});

        songTitle = fruitwork::Label::getInstance(x, y + 139, 246, 40, beatmap->cleanedTitle);
        songTitle->setFontSize(20);
        songTitle->setColor({255, 255, 255, 255});

        songArtist = fruitwork::Label::getInstance(x, y + 167, 246, 40, beatmap->artist);
        songArtist->setFontSize(18);
        songArtist->setColor({255, 255, 255, 178});

        songTime = fruitwork::Label::getInstance(x + 180, y + 110, 63, 23, "02:28"); // TODO: Get song time from beatmap
        songTime->setFontSize(24);
        songTime->setColor({255, 255, 255, 225});
        lengthBackdrop = fruitwork::Rectangle::getInstance(x + 180 - 1, y + 112 - 1, 63 + 2, 23 + 2, {0, 0, 0, 178});
    }

    void GalleryItem::start()
    {
        sprite->start();
        songTitle->start();
        songArtist->start();
        songTime->start();
    }

    void GalleryItem::draw() const
    {
        coverFrame->draw();
        coverBackdrop->draw();

        sprite->draw();
        songTitle->draw();
        songArtist->draw();

        lengthBackdrop->draw();
        songTime->draw();
    }

    void GalleryItem::onMouseDown(const SDL_Event &e)
    {
        sprite->onMouseDown(e);
        songTitle->onMouseDown(e);
        songArtist->onMouseDown(e);
        songTime->onMouseDown(e);
    }

    void GalleryItem::onMouseUp(const SDL_Event &e)
    {
        sprite->onMouseUp(e);
        songTitle->onMouseUp(e);
        songArtist->onMouseUp(e);
        songTime->onMouseUp(e);
    }

    void GalleryItem::update()
    {
        sprite->update();
        songTitle->update();
        songArtist->update();
        songTime->update();
    }

    GalleryItem::~GalleryItem()
    {
        delete sprite; // this will NOT destroy the background texture since it is not owned by the sprite
        delete songTitle; // this will destroy the title texture since it is owned by the label, etc
        delete songArtist;
        delete songTime;

        delete coverBackdrop;
        delete coverFrame;
        delete lengthBackdrop;
    }

} // yuzu