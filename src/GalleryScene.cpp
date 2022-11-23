#include <iostream>
#include "GalleryScene.h"
#include "Label.h"
#include "InputField.h"
#include "Sprite.h"
#include "Button.h"
#include "GameScene.h"
#include "GalleryItem.h"
#include <ResourceManager.h>
#include <filesystem>

namespace yuzu
{

    bool GalleryScene::enter()
    {
        bool success = true;
        std::cout << "GalleryScene::enter()" << std::endl;

        if (beatmaps.empty())
            loadBeatmaps();

        fruitwork::Button *buttonNextPage = fruitwork::Button::getInstance(10, 842, 240, 48, "Next page");
        buttonNextPage->registerCallback([](fruitwork::Button *src)
                                         {
                                             std::cout << "Next page" << std::endl;
                                             auto *scene = GalleryScene::get_instance();
                                             scene->addGalleryItems(1);
                                         });

        fruitwork::Sprite *background = fruitwork::Sprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight,
                                                                       fruitwork::ResourceManager::getTexturePath("background.png"));


        add_component(background);
        add_component(buttonNextPage);

        addGalleryItems(0);

        return success;
    }

    void GalleryScene::addGalleryItems(int page)
    {
        for (auto &item: galleryItems)
        {
            remove_component(item);
            // TODO: delete items, but the main loop will still try to call them and crash
        }

        galleryItems.clear();

        static const int w = 246;
        static const int h = 200;
        static const int padding = 36;

        int x = padding;
        int y = padding;
        for (int i = 0; i < 6; i++)
        {
            // add 6 items to the gallery, in a 2x3 grid
            if (page * 6 + i >= beatmaps.size())
                break;

            Beatmap *beatmap = beatmaps[page * 6 + i][0];
            if (beatmap == nullptr)
                break;

            GalleryItem *item = GalleryItem::getInstance(beatmaps[(page * 6) + i][0], x, y);
            galleryItems.push_back(item);
            add_component(item);

            x += w + padding;
            if (i % 2 == 1)
            {
                x = padding;
                y += h + padding;
            }
        }
    }

    bool GalleryScene::exit()
    {
        bool success = true;
        std::cout << "Exiting GalleryScene" << std::endl;

        for (auto &c: components)
        {
            delete c;
        }

        return success;
    }

    GalleryScene::GalleryScene()
    =
    default;

    GalleryScene *GalleryScene::get_instance()
    {
        return &instance;
    }

    void GalleryScene::loadBeatmaps()
    {
        // read all beatmaps from the beatmaps folder
        std::string beatmapPath = constants::gResPath + "beatmaps/";
        std::vector<std::string> beatmapSetPathNames;

        for (const auto &entry: std::filesystem::directory_iterator(beatmapPath))
            beatmapSetPathNames.push_back(entry.path().filename().string());


        for (auto &beatmapSetPathName: beatmapSetPathNames)
        {
            std::string beatmapSetPath = beatmapPath + beatmapSetPathName + "/";
            std::vector<std::string> beatmapPathNames;

            for (const auto &entry: std::filesystem::directory_iterator(beatmapSetPath))
            {
                if (entry.path().extension() == ".osu")
                    beatmapPathNames.push_back(entry.path().filename().string());
            }

            std::vector<Beatmap *> beatmapsInSet;

            for (auto &beatmapPathName: beatmapPathNames)
            {
                Beatmap *beatmap = Beatmap::loadBeatmap(beatmapSetPath + beatmapPathName);
                beatmapsInSet.push_back(beatmap);
            }

            beatmaps.push_back(beatmapsInSet);
        }

        beatmaps;
    }

    GalleryScene GalleryScene::instance;

    Beatmap *currentBeatmap = nullptr;
    std::vector<std::vector<Beatmap *>> beatmaps = {};

} // yuzu