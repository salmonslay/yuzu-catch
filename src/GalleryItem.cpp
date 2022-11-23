#include "GalleryItem.h"

namespace yuzu
{
    GalleryItem *GalleryItem::getInstance(Beatmap *beatmap, int x, int y)
    {
        return new GalleryItem(beatmap, x, y);
    }

    GalleryItem::GalleryItem(Beatmap *beatmap, int x, int y)
            : Component(x, y, 246, 199)
    {
        sprite = fruitwork::ResponsiveSprite::getInstance(x, y, 246, 139, beatmap->getBackgroundTexture());

        songTitle = fruitwork::Label::getInstance(x, y + 139, 246, 30, beatmap->title);
        songTitle->setFontSize(18);
        songTitle->setColor({255, 255, 255, 255});

        songArtist = fruitwork::Label::getInstance(x, y + 169, 246, 30, beatmap->artist);
        songArtist->setFontSize(16);
        songArtist->setColor({255, 255, 255, 178});

        songTime = fruitwork::Label::getInstance(x + 175, y + 110, 66, 29, "02:28"); // TODO: Get song time from beatmap
        songTime->setFontSize(24);
    }

    void GalleryItem::draw() const
    {
        sprite->draw();
        songTitle->draw();
        songArtist->draw();
        songTime->draw();
    }

    GalleryItem::~GalleryItem()
    {
        delete sprite; // this will NOT destroy the background texture since it is not owned by the sprite
        delete songTitle; // this will destroy the title texture since it is owned by the label, etc
        delete songArtist;
        delete songTime;
    }

} // yuzu