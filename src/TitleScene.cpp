#include <iostream>
#include <SDL_image.h>
#include "Button.h"
#include "TitleScene.h"
#include "System.h"
#include "Label.h"
#include "ResponsiveSprite.h"
#include "InputField.h"
#include "GalleryScene.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "TestSceneIndex.h"

namespace yuzu
{
    bool TitleScene::enter()
    {
        bool success = true;
        SDL_Log("Entering TitleScene...");


        fruitwork::Label *label = fruitwork::Label::getInstance(0, 50, constants::gScreenWidth, constants::gScreenHeight, "yuzu!catch");
        label->setColor({255, 204, 242, 255});
        label->setAlignment(fruitwork::Label::Alignment::CENTER);
        label->setFontSize(100);
        addComponent(label, 0);

        fruitwork::Button *startButton = fruitwork::Button::getInstance(400, 400, 400, 48, "Start Game");
        startButton->registerCallback([](fruitwork::Button *src)
                                      {
                                          fruitwork::sys.setNextScene(GalleryScene::getInstance());
                                      });
        startButton->setColor({255, 204, 232, 255});
        addComponent(startButton, 1);

        fruitwork::Button *exitButton = fruitwork::Button::getInstance(400, 500, 400, 48, "Exit Game");
        exitButton->registerCallback([](fruitwork::Button *src)
                                     {
                                         SDL_Event quitEvent;
                                         quitEvent.type = SDL_QUIT;
                                         SDL_PushEvent(&quitEvent);
                                     });
        exitButton->setColor({255, 204, 232, 255});
        addComponent(exitButton, 1);

        fruitwork::Sprite *background = fruitwork::Sprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight,
                                                                       fruitwork::ResourceManager::getTexturePath("background.png"));
        addComponent(background, -10);

        // test scene button
        fruitwork::Button *testButton = fruitwork::Button::getInstance(10, 842, 240, 48, "Visual tests...");
        testButton->registerCallback([](fruitwork::Button *src)
                                     {
                                         fruitwork::sys.setNextScene(fruitwork::TestSceneIndex::getInstance());
                                     });
        testButton->setColor({255, 255, 255, 128});
        testButton->setTextColor({0, 0, 0, 128});
        addComponent(testButton, 1);

        return success;
    }

    bool TitleScene::exit()
    {
        bool success = true;
        SDL_Log("TitleScene::exit() cleaning up");

        for (auto &c: components)
            removeComponent(c, true);

        return success;
    }

    TitleScene::TitleScene()
    =
    default;

    TitleScene *TitleScene::getInstance()
    {
        return &instance;
    }

    void TitleScene::update()
    {

    }

    TitleScene TitleScene::instance;

} // yuzu