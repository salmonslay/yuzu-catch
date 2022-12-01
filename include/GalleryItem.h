#ifndef YUZU_CATCH_GALLERYITEM_H
#define YUZU_CATCH_GALLERYITEM_H


#include "Component.h"
#include "ResponsiveSprite.h"
#include "Label.h"
#include "Beatmap.h"
#include "ImageButton.h"
#include "Rectangle.h"

namespace yuzu
{
    /**
     * A gallery item is one of the six cards in the gallery.
     * It displays the name of the beatmap, the artist, and the background.
     */
    class GalleryItem : public fruitwork::Component {
    public:
        static GalleryItem *getInstance(Beatmap *beatmap, int x, int y, int beatmapIndex);

        void onMouseDown(const SDL_Event &) override;

        void onMouseUp(const SDL_Event &) override;

        void start() override;

        void update() override;

        void draw() const override;

        ~GalleryItem() override;

    protected:
        GalleryItem(Beatmap *beatmap, int x, int y, int beatmapIndex);

    private:
        fruitwork::Rectangle *coverBackdrop;
        fruitwork::Rectangle *coverFrame;
        fruitwork::Rectangle *lengthBackdrop;

        fruitwork::ImageButton *coverButton;
        fruitwork::Label *songTitle;
        fruitwork::Label *songArtist;
        fruitwork::Label *songTime;
        int beatmapIndex;
    };

} // yuzu
#endif //YUZU_CATCH_GALLERYITEM_H
