#include <iostream>
#include <Session.h>
#include <Label.h>
#include <Button.h>

using namespace fruitwork;

Label *label = Label::getInstance(10, 10, 200, 100, "0");

int main(int argc, char **argv)
{
    Session session;


    Button *button = Button::getInstance(10, 120, 200, 100, "Inc");
    button->registerCallback([](Button *source)
                             {
                                 int value = std::stoi(label->getText());
                                 label->setText(std::to_string(value + 1));
                             });

    session.add_component(label);
    session.add_component(button);
    session.run();

    return 0;
}