#include "TitleScene.h"
#include <iostream>
#include <Session.h>

fruitwork::Session ses;

int main(int argc, char **argv)
{
    std::cout << "main()" << std::endl;

    ses.run(yuzu::TitleScene::getInstance());

    return 0;
}

