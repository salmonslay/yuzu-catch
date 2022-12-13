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

        fruitwork::Button *startButton = fruitwork::Button::getInstance(50, 400, 240, 48, "Start");
        startButton->registerCallback([](fruitwork::Button *src)
                                      {
                                          std::cout << "Start button clicked" << std::endl;
                                          fruitwork::sys.setNextScene(GalleryScene::getInstance());
                                      });

        startButton->setTextColor({134, 140, 158, 255});
        startButton->setColor({226, 255, 191, 255});

        fruitwork::Label *label = fruitwork::Label::getInstance(0, 50, constants::gScreenWidth, constants::gScreenHeight, "yuzu!catch");
        label->setColor({255, 204, 242, 255});
        label->setAlignment(fruitwork::Label::Alignment::CENTER);
        label->setFontSize(100);

        fruitwork::InputField *inputField = fruitwork::InputField::getInstance(50, 300, 390, 48, "Enter your name...");

        fruitwork::Sprite *background = fruitwork::Sprite::getInstance(0, 0, constants::gScreenWidth, constants::gScreenHeight, fruitwork::ResourceManager::getTexturePath("background.png"));

        // test scene button
        fruitwork::Button *testButton = fruitwork::Button::getInstance(10, 842, 240, 48, "Visual tests...");
        testButton->registerCallback([](fruitwork::Button *src)
                                     {
                                         fruitwork::sys.setNextScene(fruitwork::TestSceneIndex::getInstance());
                                     });
        testButton->setColor({255, 255, 255, 128});
        testButton->setTextColor({0, 0, 0, 128});

        addComponent(background);
        addComponent(label);
        addComponent(startButton);
        addComponent(inputField);
        addComponent(testButton);

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