#ifndef YUZU_CATCH_GALLERYITEM_H
#define YUZU_CATCH_GALLERYITEM_H


#include "Component.h"
#include "ResponsiveSprite.h"
#include "Label.h"
#include "Beatmap.h"
#include "ImageButton.h"

namespace yuzu
{
    class GalleryItem : public fruitwork::Component {
    public:
        static GalleryItem *getInstance(Beatmap *beatmap, int x, int y);

        void onMouseDown(const SDL_Event &) override;

        void onMouseUp(const SDL_Event &) override;

        void start() override;

        void update() override;

        void draw() const override;

        ~GalleryItem() override;

    protected:
        GalleryItem(Beatmap *beatmap, int x, int y);

    private:

        SDL_Rect coverBackdropRect;
        SDL_Rect coverFrameRect;
        SDL_Rect lengthBackdropRect;
        fruitwork::ImageButton *sprite;
        fruitwork::Label *songTitle;
        fruitwork::Label *songArtist;
        fruitwork::Label *songTime;
    };

} // yuzu
#endif //YUZU_CATCH_GALLERYITEM_H
