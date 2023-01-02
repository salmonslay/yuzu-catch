#ifndef YUZU_CATCH_SCORE_DISPLAY_H
#define YUZU_CATCH_SCORE_DISPLAY_H

#include "Component.h"
#include "Score.h"
#include "Label.h"
#include "JuiceDrop.h"
#include "Rectangle.h"
#include "Button.h"

namespace yuzu
{
    // @see https://www.figma.com/file/jqsfPXPSVrxE5RAWfIUFfc/yuzu?node-id=0%3A1&t=umyUrnbfA1Bzif5H-0
    class ScoreDisplay : public fruitwork::Component {
    public:
        static ScoreDisplay *getInstance(int x, int y, int w, int h, Score *score);

        void draw() const override;

        void update() override;

        void onMouseDown(const SDL_Event &) override;

        void onMouseUp(const SDL_Event &) override;

        ~ScoreDisplay() override;

    protected:
        ScoreDisplay(int x, int y, int w, int h, Score *score);

    private:
        Score *score;

        fruitwork::Rectangle *background;

        fruitwork::Sprite *rank;
        fruitwork::Label *titleLabel;

        SDL_Texture *fruitBaseTexture = nullptr;
        SDL_Texture *fruitOverlayTexture = nullptr;
        fruitwork::Sprite *hitFruitBase;
        fruitwork::Sprite *hitFruitOverlay;
        fruitwork::Sprite *missFruitBase;
        fruitwork::Sprite *missFruitOverlay;
        fruitwork::Sprite *hitIcon;
        fruitwork::Sprite *missIcon;
        fruitwork::Label *hitLabel;
        fruitwork::Label *missLabel;

        fruitwork::Sprite *hitBananaBase;
        fruitwork::Sprite *hitBananaOverlay;
        fruitwork::Label *hitBananaLabel;

        fruitwork::Label *scoreLabel;
        fruitwork::Label *accLabel;

        fruitwork::Button *backButton;
    };

} // yuzu

#endif //YUZU_CATCH_SCORE_DISPLAY_H
