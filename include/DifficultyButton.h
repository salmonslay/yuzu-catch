#ifndef YUZU_CATCH_DIFFICULTYBUTTON_H
#define YUZU_CATCH_DIFFICULTYBUTTON_H

#include <vector>
#include "Button.h"
#include "Sprite.h"
#include "Label.h"
#include "Beatmap.h"

namespace yuzu
{

    class DifficultyButton : public fruitwork::Button {
    public:
        static DifficultyButton *getInstance(int x, int y, int w, int h, Beatmap* beatmap);

        void draw() const override;

        ~DifficultyButton() override;

    protected:
        DifficultyButton(int x, int y, int w, int h, Beatmap* beatmap);

    private:
        fruitwork::Label *difficultyLabel;

        std::string difficultyName;
        int starCount;
        SDL_Color starModColor;
        std::vector<fruitwork::Sprite*> stars;
        SDL_Texture *starTexture = nullptr;

        Beatmap* beatmap;
    };

} // yuzu

#endif //YUZU_CATCH_DIFFICULTYBUTTON_H
