#include <iostream>
#include "GalleryScene.h"
#include "Label.h"
#include "InputField.h"
#include "Sprite.h"
#include "Button.h"
#include "GameScene.h"
#include "GalleryItem.h"
#include "ImageButton.h"
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

        fruitwork::ImageButton *buttonPreviousPage = fruitwork::ImageButton::getInstance(
                92, 705, 128, 128, fruitwork::ResourceManager::getTexturePath("arrow.png"));
        buttonPreviousPage->registerCallback([](fruitwork::Button *src)
                                             {
                                                 GalleryScene::get_instance()->changePage(false);
                                             });
        buttonPreviousPage->setFlip(SDL_FLIP_HORIZONTAL);

        fruitwork::ImageButton *buttonNextPage = fruitwork::ImageButton::getInstance(
                374, 705, 128, 128, fruitwork::ResourceManager::getTexturePath("arrow.png"));
        buttonNextPage->registerCallback([](fruitwork::Button *src)
                                         {
                                             GalleryScene::get_instance()->changePage(true);
                                         });

        pageLabel = fruitwork::Label::getInstance(223, 741, 149, 79, "1/1");
        pageLabel->setFontSize(36);
        pageLabel->setAlignment(fruitwork::Label::Alignment::CENTER);
        pageLabel->setColor({255, 255, 255, 255});

        fruitwork::Sprite *background = fruitwork::Sprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight,
                                                                       fruitwork::ResourceManager::getTexturePath("background.png"));

        galleryFocus = GalleryFocus::getInstance(637, 36);


        add_component(background);
        add_component(buttonPreviousPage);
        add_component(buttonNextPage);
        add_component(pageLabel);
        add_component(galleryFocus);

        addGalleryItems(currentPage);

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

            int beatmapIndex = (page * 6) + i;
            GalleryItem *item = GalleryItem::getInstance(beatmaps[beatmapIndex][0], x, y, beatmapIndex);
            galleryItems.push_back(item);
            add_component(item);

            x += w + padding;
            if (i % 2 == 1)
            {
                x = padding;
                y += h + padding;
            }
        }

        pageLabel->setText(std::to_string(page + 1) + "/" + std::to_string((beatmaps.size() / 6) + 1));
    }

    void GalleryScene::changePage(bool next)
    {
        currentPage += next ? 1 : -1;

        if (currentPage < 0)
            currentPage = beatmaps.size() / 6;
        else if (currentPage > beatmaps.size() / 6)
            currentPage = 0;

        addGalleryItems(currentPage);
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
    }

    void GalleryScene::setSelectedBeatmap(int index)
    {
        currentBeatmap = beatmaps[index];
        SDL_Log("Selected beatmap %s", currentBeatmap[0]->title.c_str());
        galleryFocus->setBeatmap(currentBeatmap);
    }

    GalleryScene GalleryScene::instance;

    std::vector<Beatmap *> currentBeatmap = {};
    std::vector<std::vector<Beatmap *>> beatmaps = {};

} // yuzu