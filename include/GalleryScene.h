#ifndef YUZU_CATCH_GALLERYSCENE_H
#define YUZU_CATCH_GALLERYSCENE_H

#include "Scene.h"
#include "Beatmap.h"
#include "GalleryItem.h"

namespace yuzu
{

    class GalleryScene : public fruitwork::Scene {
    public:
        static GalleryScene *get_instance();

        bool enter() override;

        bool exit() override;

    private:
        static GalleryScene instance;
        int currentPage = 0;

        GalleryScene();

        std::vector<GalleryItem *> galleryItems{};
        fruitwork::Label *pageLabel;

        /**
         * Loads in all beatmaps and fills the beatmap list.
         */
        static void loadBeatmaps();

        /**
         * Adds 6 beatmaps to the gallery.
         * @param page The page to add, 0 is the first page.
         */
        void addGalleryItems(int page);

        /**
         * Changes page of the gallery.
         * @param next True if the next page should be shown, false if the previous page should be shown.
         */
        void changePage(bool next);
    };


    struct BeatmapSet {
        std::vector<Beatmap *> beatmaps;
        std::string title;
        std::string artist;
        std::string creator;
        int songLength;
        SDL_Texture *background;
    };

    /**
     * The currently picked beatmap
     */
    extern Beatmap *currentBeatmap;

    /**
     * 2D array of beatmaps.
     * The first dimension is the beatmap set, the second dimension is the beatmap itself.
     */
    extern std::vector<std::vector<Beatmap *>> beatmaps;

} // yuzu

#endif //YUZU_CATCH_GALLERYSCENE_H