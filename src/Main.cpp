#include "TitleScene.h"
#include "GalleryScene.h"
#include <iostream>
#include <Session.h>

namespace yuzu
{
    fruitwork::Session ses;
}

using namespace yuzu;

int main(int argc, char **argv)
{
    std::cout << "main()" << std::endl;

    ses.run(TitleScene::getInstance());

    // clean up beatmap database
    std::cout << "Cleaning up beatmap database..." << std::endl;
    int i = 0;
    for (const std::vector<Beatmap *> &beatmap: beatmaps)
        for (Beatmap *difficulty: beatmap)
        {
            delete difficulty;
            i++;
        }
    std::cout << "Cleaned up " << i << " beatmaps." << std::endl;

    return 0;
}

