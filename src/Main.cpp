#include "TitleScene.h"
#include <iostream>
#include <Session.h>

using namespace fruitwork;

int main(int argc, char **argv)
{
    std::cout << "main()" << std::endl;

    Session ses;

    ses.run(yuzu::TitleScene::get_instance());

    return 0;
}