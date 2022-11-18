#include <iostream>
#include "Button.h"
#include "TitleScene.h"
#include "System.h"
#include "ExitScene.h"
#include "Label.h"
#include "ResponsiveSprite.h"

namespace yuzu
{
    bool TitleScene::enter()
    {
        bool success = true;
        std::cout << "TitleScene::enter()" << std::endl;

        fruitwork::Button *startButton = fruitwork::Button::getInstance(50, 400, 390, 100, "Start");
        startButton->registerCallback([](fruitwork::Button *src)
                                      {
                                          std::cout << "Start button clicked" << std::endl;
                                          fruitwork::sys.setNextScene(fruitwork::ExitScene::get_instance());
                                      });

        startButton->setTextColor({255, 0, 0, 255});
        startButton->setColor({0, 255, 0, 255});

        fruitwork::Label *label = fruitwork::Label::getInstance(0, 50, 1200, 900, "yuzu!catch");
        label->setColor({255, 204, 242, 255});
        label->setAlignment(fruitwork::Label::Alignment::CENTER);
        label->setFontSize(100);
        label->setFont("Kepler 296.ttf");

        fruitwork::Sprite *background = fruitwork::Sprite::getInstance(0, 0, 1200, 900, "background.png");

        add_component(background);
        add_component(label);
        add_component(startButton);

        return success;
    }

    bool TitleScene::exit()
    {
        bool success = true;
        std::cout << "Exiting TitleScene" << std::endl;

        for (auto &c: components)
        {
            delete c;
        }

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