#include <iostream>
#include "game.h"

int main(int argc, const char* argv[])
{
    std::cout << "Hello, world!" << std::endl;

    Game game;

    Event::Join join{1};
    game.pushEvent(Event::JOIN, &join);
    Event::Join join2{2};
    game.pushEvent(Event::JOIN, &join2);
    game.handleEvents();
    return 0;
}