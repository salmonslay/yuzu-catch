#include <iostream>
#include "Button.h"
#include "TitleScene.h"
#include "System.h"
#include "ExitScene.h"

namespace yuzu
{
    bool TitleScene::enter()
    {
        bool success = true;
        std::cout << "TitleScene::enter()" << std::endl;

        startButton = fruitwork::Button::getInstance(500, 500, 100, 100, "Start");
        startButton->registerCallback([](fruitwork::Button *src)
                                      {
                                          std::cout << "Start button clicked" << std::endl;
                                          fruitwork::sys.setNextScene(fruitwork::ExitScene::get_instance());
                                      });

        logo = fruitwork::Sprite::getInstance(100, 200, 100, 100, "logo.png");

        add_component(startButton);
        add_component(logo);

        if (startButton == nullptr || logo == nullptr)
            success = false;

        return success;
    }

    bool TitleScene::exit()
    {
        bool success = true;
        std::cout << "Exiting TitleScene" << std::endl;

        delete startButton;

        delete logo;

        return success;
    }

    TitleScene::TitleScene()
    =
    default;

    TitleScene *TitleScene::get_instance()
    {
        return &instance;
    }

    void TitleScene::update()
    {

    }

    TitleScene TitleScene::instance;

} // yuzu