#ifndef YUZU_CATCH_GALLERYFOCUS_H
#define YUZU_CATCH_GALLERYFOCUS_H

#include "Component.h"
#include "Beatmap.h"
#include "Label.h"
#include "ResponsiveSprite.h"
#include "Button.h"

namespace yuzu
{
    /**
     * A gallery focus is the right part of the gallery focusing a single beatmap.
     * It shows the background, the name of the beatmap, the artist, the creator, the length of the song, and the difficulties as buttons.
     */
    class GalleryFocus : public fruitwork::Component {
    public:
        static GalleryFocus *getInstance(int x, int y);

        void start() override;

        void update() override;

        void draw() const override;

        void onMouseDown(const SDL_Event &) override;

        void onMouseUp(const SDL_Event &) override;

        void setBeatmap(std::vector<Beatmap *> beatmapSet);

        ~GalleryFocus() override;

    protected:
        GalleryFocus(int x, int y);

    private:
        SDL_Rect coverBackdropRect;
        SDL_Rect lengthBackdropRect;

        fruitwork::ResponsiveSprite *background;
        fruitwork::Label *songTitle;
        fruitwork::Label *songArtist;
        fruitwork::Label *songCreator;
        fruitwork::Label *songTime;

        std::vector<fruitwork::Button *> difficultyButtons;
    };

} // yuzu

#endif //YUZU_CATCH_GALLERYFOCUS_H
