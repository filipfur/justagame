#include <iostream>
#include "game.h"
#include "client.h"
#include "clientcontroller.h"
#include "testcontroller.h"

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

    Client clientA{&clientAIO};
    Client clientB{&clientBIO};

    TestController clientAController{&clientA};
    TestController clientBController{&clientB};

    const auto playRandomCard = [](Scheduler* scheduler) {
        auto controller = dynamic_cast<ClientController*>(scheduler);
        Player* player = controller->client()->player();
        Event::PlayCard playCard;
        playCard.cardId = player->cardByIndex((rand() % 20) % player->cardCount()).cardId;
        playCard.position.x = rand() % 20;
        playCard.position.y = rand() % 20;
        controller->client()->pushEvent(Event::PLAY_CARD, &playCard);
        return false;
    };

    const auto passTurn = [](Scheduler* scheduler) {
        auto controller = dynamic_cast<ClientController*>(scheduler);
        controller->client()->pushEvent(Event::PASS, nullptr);
        return false;
    };

    clientAController.setOnNewTurnCallback([&playRandomCard, &passTurn](TestController* controller, uint32_t turnId) {
        std::cout << "Client A turn: " << turnId << std::endl;
        switch(turnId)
        {
            case 0:
                controller->startDelayed(playRandomCard, 2.0f);
                break;
            case 2:
                controller->startDelayed(playRandomCard, 4.0f);
                controller->startDelayed(passTurn, 5.0f);
                break;
        }
    });

    clientBController.setOnNewTurnCallback([&playRandomCard, &passTurn](TestController* controller, uint32_t turnId) {
        std::cout << "Client B turn: " << turnId << std::endl;
        switch(turnId)
        {
            case 1:
                controller->startDelayed(playRandomCard, 4.0f);
                controller->startDelayed(passTurn, 6.0f);
                break;
            case 3:
                controller->startDelayed(passTurn, 2.0f);
                break;
        }
    });

    Game game{};
    game.addConnection(&serverClientAIO);
    game.addConnection(&serverClientBIO);

    clientA.startPoll();
    clientB.startPoll();

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));


    const auto createGame = [](Scheduler* scheduler)
    {
        auto controller = dynamic_cast<ClientController*>(scheduler);
        Event::Settings settings;
        settings.numberOfPlayers = 2;
        settings.mapSize.x = 20;
        settings.mapSize.y = 20;
        controller->client()->pushEvent(Event::SETTINGS, &settings);
        return false;
    };
    const auto joinGame = [](Scheduler* scheduler)
    {
        auto controller = dynamic_cast<ClientController*>(scheduler);
        controller->client()->joinGame();
        return false;
    };

    clientAController.startDelayed(createGame, 0.1f);
    clientAController.startDelayed(joinGame, 0.5f);
    clientBController.startDelayed(joinGame, 0.7f);

    float multiplier{4.0f};
    float dt{10.0f * multiplier * 1e-3f};
    for(int i{0}; i < 1000; ++i)
    {
        try
        {
            clientA.handleEvents();
            clientB.handleEvents();
            game.handleEvents();
            clientAController.update(dt);
            clientBController.update(dt);
            game.update(dt);
        }
        catch(std::runtime_error e)
        {
            std::cerr << "runtime_error: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    game.exit();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    clientA.handleEvents();
    clientB.handleEvents();

    game.close();

    return 0;
}