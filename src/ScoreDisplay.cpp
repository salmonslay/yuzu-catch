#include <SDL_image.h>
#include "ScoreDisplay.h"
#include "ResourceManager.h"
#include "GalleryScene.h"

namespace yuzu
{

    ScoreDisplay *ScoreDisplay::getInstance(int x, int y, int w, int h, Score *score)
    {
        return new ScoreDisplay(x, y, w, h, score);
    }

    ScoreDisplay::ScoreDisplay(int x, int y, int w, int h, Score *score) : Component(x, y, w, h)
    {
        std::string grade = score->getGrade();
        background = fruitwork::Rectangle::getInstance(x, y, w, h, {0, 0, 0, 200});

        rank = fruitwork::Sprite::getInstance(450, 0, 300, 300, fruitwork::ResourceManager::getTexturePath("ranking-" + grade + ".png"));

        titleLabel = fruitwork::Label::getInstance(336, 281, 528, 45, "You achieved rank " + grade + "!");
        titleLabel->setFontSize(36);
        titleLabel->setColor({255, 255, 255, 255});
        titleLabel->setAlignment(fruitwork::Label::Alignment::CENTER);

        fruitBaseTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-apple.png").c_str());
        fruitOverlayTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-apple-overlay.png").c_str());

        hitFruitBase = fruitwork::Sprite::getInstance(526, 351, 64, 64, fruitBaseTexture);
        hitFruitBase->setColorMod({204, 255, 143});
        hitFruitOverlay = fruitwork::Sprite::getInstance(526, 351, 64, 64, fruitOverlayTexture);
        missFruitBase = fruitwork::Sprite::getInstance(526, 415, 64, 64, fruitBaseTexture);
        missFruitBase->setColorMod({255, 143, 143});
        missFruitOverlay = fruitwork::Sprite::getInstance(526, 415, 64, 64, fruitOverlayTexture);

        hitIcon = fruitwork::Sprite::getInstance(552, 376, 48, 48, fruitwork::ResourceManager::getTexturePath("section-pass.png"));
        missIcon = fruitwork::Sprite::getInstance(552, 440, 48, 48, fruitwork::ResourceManager::getTexturePath("section-fail.png"));

        hitLabel = fruitwork::Label::getInstance(614, 356, 390, 64, std::to_string(score->getCaughtFruits()));
        hitLabel->setFontSize(48);
        hitLabel->setColor({255, 255, 255, 255});

        missLabel = fruitwork::Label::getInstance(614, 420, 390, 64, std::to_string(score->getMissedFruits()));
        missLabel->setFontSize(48);
        missLabel->setColor({255, 255, 255, 255});

        hitBananaBase = fruitwork::Sprite::getInstance(526, 488, 64, 64, fruitwork::ResourceManager::getTexturePath("fruit-bananas.png"));
        hitBananaBase->setColorMod({255, 240, 0});
        hitBananaOverlay = fruitwork::Sprite::getInstance(526, 488, 64, 64, fruitwork::ResourceManager::getTexturePath("fruit-bananas-overlay.png"));

        hitBananaLabel = fruitwork::Label::getInstance(614, 493, 390, 64, score->getBananaAccuracyString());
        hitBananaLabel->setFontSize(48);
        hitBananaLabel->setColor({255, 255, 255, 255});

        scoreLabel = fruitwork::Label::getInstance(336, 578, 528, 45, "Score: " + std::to_string(score->getDisplayScore()));
        scoreLabel->setFontSize(36);
        scoreLabel->setColor({255, 255, 255, 255});
        scoreLabel->setAlignment(fruitwork::Label::Alignment::CENTER);

        accLabel = fruitwork::Label::getInstance(336, 623, 528, 45, "Accuracy: " + score->getAccuracyString());
        accLabel->setFontSize(36);
        accLabel->setColor({255, 255, 255, 255});
        accLabel->setAlignment(fruitwork::Label::Alignment::CENTER);

        comboLabel = fruitwork::Label::getInstance(336, 668, 528, 45, "Combo: " + std::to_string(score->getMaxCombo()) + "x");
        comboLabel->setFontSize(36);
        comboLabel->setColor({255, 255, 255, 255});
        comboLabel->setAlignment(fruitwork::Label::Alignment::CENTER);

        // centered back button
        backButton = fruitwork::Button::getInstance(400, 768, 400, 45, "Back to Menu");
        backButton->registerCallback([](fruitwork::Button *src)
                                     {
                                         fruitwork::sys.setNextScene(GalleryScene::getInstance());
                                     });
        backButton->setColor({204, 255, 143});
    }

    void ScoreDisplay::draw() const
    {
        background->draw();

        rank->draw();
        titleLabel->draw();

        hitFruitBase->draw();
        hitFruitOverlay->draw();
        missFruitBase->draw();
        missFruitOverlay->draw();

        hitIcon->draw();
        missIcon->draw();

        hitLabel->draw();
        missLabel->draw();

        hitBananaBase->draw();
        hitBananaOverlay->draw();
        hitBananaLabel->draw();

        scoreLabel->draw();
        accLabel->draw();
        comboLabel->draw();

        backButton->draw();
    }

    ScoreDisplay::~ScoreDisplay()
    {
        delete background;
        delete rank;
        delete titleLabel;
        delete hitFruitBase;
        delete hitFruitOverlay;
        delete missFruitBase;
        delete missFruitOverlay;
        delete hitIcon;
        delete missIcon;
        delete hitLabel;
        delete missLabel;
        delete hitBananaBase;
        delete hitBananaOverlay;
        delete hitBananaLabel;
        delete scoreLabel;
        delete accLabel;
        delete comboLabel;
        delete backButton;
    }

    void ScoreDisplay::update()
    {
        backButton->update();
    }

    void ScoreDisplay::onMouseDown(const SDL_Event &e)
    {
        backButton->onMouseDown(e);
    }

    void ScoreDisplay::onMouseUp(const SDL_Event &e)
    {
        backButton->onMouseUp(e);
    }
} // yuzu