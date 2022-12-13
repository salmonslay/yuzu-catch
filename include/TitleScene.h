#ifndef YUZU_CATCH_TITLESCENE_H
#define YUZU_CATCH_TITLESCENE_H

#include <Scene.h>
#include <Button.h>
#include <Sprite.h>

namespace yuzu
{

    class TitleScene : public fruitwork::Scene {
    public:
        static TitleScene *getInstance();

        bool enter() override;

        bool exit() override;

        void update() override;


    private:
        static TitleScene instance;

        TitleScene();
    };

} // yuzu

#endif //YUZU_CATCH_TITLESCENE_H