#include <SDL_image.h>
#include "Catcher.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "System.h"

namespace yuzu
{

    Catcher *Catcher::getInstance(int x, int y, int w, int h)
    {
        return new Catcher(x, y, w, h);
    }

    Catcher::Catcher(int x, int y, int w, int h) : Sprite(x, y, w, h, nullptr)
    {
        textureIdle = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-idle.png").c_str());
        textureKiai = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-kiai.png").c_str());
        textureFail = IMG_LoadTexture(fruitwork::sys.get_renderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-fail.png").c_str());
    }

    void Catcher::start()
    {
        fruitwork::Sprite::start();
        setTexture(textureIdle);
    }

    void Catcher::update()
    {
        fruitwork::Sprite::update();

        if (moveRight)
            moveBy(catcherSpeed * (isDash ? 2 : 1));
        if (moveLeft)
            moveBy(-catcherSpeed * (isDash ? 2 : 1));
    }

    Catcher::~Catcher()
    {
        fruitwork::Sprite::~Sprite();
        SDL_DestroyTexture(textureIdle);
        SDL_DestroyTexture(textureKiai);
        SDL_DestroyTexture(textureFail);
    }

    void Catcher::onKeyDown(const SDL_Event &e)
    {
        if (e.key.keysym.sym == SDLK_LSHIFT)
            isDash = true;
        else if (e.key.keysym.sym == SDLK_LEFT)
            moveLeft = true;
        else if (e.key.keysym.sym == SDLK_RIGHT)
            moveRight = true;
    }

    void Catcher::onKeyUp(const SDL_Event &e)
    {
        if (e.key.keysym.sym == SDLK_LSHIFT)
            isDash = false;
        else if (e.key.keysym.sym == SDLK_LEFT)
            moveLeft = false;
        else if (e.key.keysym.sym == SDLK_RIGHT)
            moveRight = false;
    }

    void Catcher::moveBy(int x)
    {
        x = static_cast<int>(x * constants::speedMod);

        SDL_Rect rect = get_rect();
        rect.x += x;
        if (rect.x < 0)
            rect.x = 0;
        if (rect.x + rect.w > constants::gScreenWidth)
            rect.x = constants::gScreenWidth - rect.w;

        set_rect(rect);
    }

} // yuzu