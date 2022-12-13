#include "TitleScene.h"
#include <iostream>
#include <Session.h>
#include <Beatmap.h>

using namespace fruitwork;

inline yuzu::Beatmap *currentBeatmap = nullptr;

int main(int argc, char **argv)
{
    std::cout << "main()" << std::endl;

    Session ses;

    ses.run(yuzu::TitleScene::getInstance());

    return 0;
}