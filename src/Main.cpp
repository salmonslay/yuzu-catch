#include "TitleScene.h"
#include <iostream>
#include <Session.h>

namespace yuzu
{
    fruitwork::Session ses;
}

int main(int argc, char **argv)
{
    std::cout << "main()" << std::endl;

    yuzu::ses.run(yuzu::TitleScene::getInstance());

    return 0;
}

