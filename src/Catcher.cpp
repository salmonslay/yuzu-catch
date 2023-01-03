#include <SDL_image.h>
#include "Catcher.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "System.h"
#include "Component.h"
#include "HitObject.h"
#include "Fruit.h"
#include "GameScene.h"

namespace yuzu
{

    Catcher *Catcher::getInstance(int x, int y, int w, int h)
    {
        return new Catcher(x, y, w, h);
    }

    Catcher::Catcher(int x, int y, int w, int h) : Sprite(x, y, w, h, nullptr)
    {
        textureIdle = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-idle.png").c_str());
        textureKiai = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-kiai.png").c_str());
        textureFail = IMG_LoadTexture(fruitwork::sys.getRenderer(), fruitwork::ResourceManager::getTexturePath("fruit-catcher-fail.png").c_str());
    }

    void Catcher::start()
    {
        fruitwork::Sprite::start();
        setTexture(textureIdle);
    }

    void Catcher::update()
    {
        fruitwork::Component::update();

        if (moveRight)
        {
            moveBy(CATCHER_SPEED * (isDash ? 2 : 1));
            setFlip(SDL_FLIP_NONE);
        }
        if (moveLeft)
        {
            moveBy(-CATCHER_SPEED * (isDash ? 2 : 1));
            setFlip(SDL_FLIP_HORIZONTAL);
        }

        if (isDash)
        {
            if (SDL_GetTicks64() - lastPhantomSpawn > PHANTOM_INTERVAL)
            {
                spawnPhantom();
                lastPhantomSpawn = SDL_GetTicks64();
            }
        }

        if (dynamic_cast<GameScene *>(fruitwork::sys.getCurrentScene())->isKiai())
            setTexture(textureKiai);
        else
            setTexture(textureIdle);
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

        SDL_Rect rect = getRect();
        rect.x += x;
        if (rect.x < 0)
            rect.x = 0;
        if (rect.x + rect.w > constants::gScreenWidth)
            rect.x = constants::gScreenWidth - rect.w;

        setRect(rect);
    }

    SDL_Point Catcher::getPlateRange() const
    {
        SDL_Rect rect = getRect();
        return {rect.x + 20, rect.x + rect.w - 20};
    }

    void Catcher::addFruitToPlate(HitObject *ho)
    {
        ho->setState(HitObjectState::PLATED);

        // local plate hit position
        SDL_Point plateRange = getPlateRange();
        int plateLeft = plateRange.x + (plateRange.y - plateRange.x) * 0.2f; // reduce plate area by 20% towards the center
        int hitPosition = ho->getRect().x + ho->getRect().w / 2;
        int localPosition = hitPosition - plateLeft;

        addChild(ho);

        SDL_Rect r = ho->getRect();
        r.x = localPosition + r.w / 2;

        r.y = -15 - rand() % 10; // move up between 15 and 25px

        r.w /= 1.4f; // scale down slightly
        r.h /= 1.4f;

        for (size_t i = getChildren().size() - 1; i > 0; --i)
        {
            fruitwork::Component *c = getChildren()[i];
            if (c == ho)
                continue;

            auto *ho2 = dynamic_cast<HitObject *>(c);

            // this checks for overlaps but completely ignores the y-axis to allow fruits to stack higher even if the detection is far under (the plate)
            // i was considering editing Sprite::rectCollidesWith to take an optional y-axis parameter,
            // but that felt kinda hacky and pointless
            const int threshold = 35;
            if (ho->getRect().x + ho->getRect().w >= ho2->getRect().x + threshold &&
                ho->getRect().x <= ho2->getRect().x + ho2->getRect().w - threshold)
            {
                r.y = ho2->getLocalRect().y - (rand() % 15 + 15); // between 15-30px above the other fruit
                break;
            }
        }

        ho->setRect(r);
    }

    void Catcher::explodePlate()
    {
        for (auto c: getChildren())
        {
            auto f = dynamic_cast<Fruit *>(c);
            if (f == nullptr)
                continue; // just to be safe

            removeChild(c);

            auto *b = fruitwork::PhysicsBody::getInstance(c->getRect(), 5);
            c->setPhysicsBody(b);

            SDL_Point plateRange = getPlateRange();
            int plateCenter = (plateRange.y - plateRange.x) / 2;

            b->setGravity(20);

            float xForce = (c->getRect().x - plateCenter) * 3;
            const float yForce = -5000;
            b->addForce(xForce, yForce);

            f->setFading();
        }
    }

    void Catcher::spawnPhantom()
    {
        SDL_Rect r = getRect();
        fruitwork::Sprite *phantom = fruitwork::Sprite::getInstance(r.x, r.y, r.w, r.h, spriteTexture);
        fruitwork::sys.getCurrentScene()->addComponent(phantom, zIndex() - 1);
        phantom->setAlphaMod(200);
        phantom->setColorMod({255, 168, 170});
        phantom->fadeOut(3000);
        phantom->setFlip(getFlip());
    }

} // yuzu