#ifndef YUZU_CATCH_GALLERYITEM_H
#define YUZU_CATCH_GALLERYITEM_H


#include "Component.h"
#include "ResponsiveSprite.h"
#include "Label.h"
#include "Beatmap.h"

namespace yuzu
{
    class GalleryItem : public fruitwork::Component {
    public:
        static GalleryItem *getInstance(Beatmap *beatmap, int x, int y);

        void draw() const override;

        ~GalleryItem() override;

    protected:
        GalleryItem(Beatmap *beatmap, int x, int y);

    private:

        fruitwork::ResponsiveSprite *sprite;
        fruitwork::Label *songTitle;
        fruitwork::Label *songArtist;
        fruitwork::Label *songTime;
    };

} // yuzu
#endif //YUZU_CATCH_GALLERYITEM_H
