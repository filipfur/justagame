#include <iostream>
#include "game.h"
#include "testclient.h"

int main(int argc, const char* argv[])
{
    std::cout << "Hello, world!" << std::endl;

    IO serverClientAIO;
    IO serverClientBIO;

    IO clientAIO;
    IO clientBIO;

    serverClientAIO.connect(&clientAIO);
    clientAIO.connect(&serverClientAIO);

    serverClientBIO.connect(&clientBIO);
    clientBIO.connect(&serverClientBIO);

    TestClient clientA{&clientAIO};
    TestClient clientB{&clientBIO};

    Game game{};
    game.addConnection(&serverClientAIO);
    game.addConnection(&serverClientBIO);

    clientA.startPoll();
    clientB.startPoll();

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Event::Settings settings;
    settings.numberOfPlayers = 2;
    settings.mapSize.x = 20;
    settings.mapSize.y = 20;
    clientA.pushEvent(Event::SETTINGS, &settings);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    clientA.joinGame();
    clientB.joinGame();

    float dt{10.0f * 1e-3f};
    for(int i{0}; i < 100; ++i)
    {
        try
        {
            clientA.handleEvents();
            clientB.handleEvents();
            game.handleEvents();
            clientA.update(dt);
            clientB.update(dt);
            game.update(dt);
        }
        catch(std::runtime_error e)
        {
            std::cerr << "runtime_error: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    game.exit();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    clientA.handleEvents();
    clientB.handleEvents();

    game.close();

    return 0;
}