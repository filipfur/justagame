#include "client.h"

#define LOG(str, ...) printf("%8.3f [client] " str "\n", time(), ##__VA_ARGS__)

Client::Client(IO* io) : IEvent{io}
{

}

Client::~Client() noexcept
{

}

void Client::handleGameInfo(Event::GameInfo* gameInfo)
{
    LOG("GameInfo: id=%d players=%d cols=%d rows=%d startX=%d startY=%d",
        gameInfo->clientId,
        gameInfo->settings.numberOfPlayers,
        gameInfo->settings.mapSize.x,
        gameInfo->settings.mapSize.y,
        gameInfo->position.x,
        gameInfo->position.y);
    _gameInfo = *gameInfo;
    _state = State::WAITING_FOR_START;
    printf("Time to load graphics and stuff...\n");
    startDelayed([this](Scheduler* scheduler) -> bool
    {
        if(_state != State::GAME_STARTED)
        {
            pushEvent(Event::READY, nullptr);
        }
        return _state != State::GAME_STARTED;
    }, 0.2f);
    for(int i{0}; i < goptions::startNumberOfCards; ++i)
    {
        printf("Card: cardId=%d, cardType=%d\n", gameInfo->cards[i].cardId, gameInfo->cards[i].cardType);
        _player.addCard(gameInfo->cards[i]);
    }
}

void Client::handleExit(Event::Exit* exit)
{
    LOG("Exit: %s", exit->message);
    close();
}

void Client::handleStart()
{
    LOG("Start:");
    if(_state != State::WAITING_FOR_START)
    {
        printf("Warning: Wrong state: %d", static_cast<int>(_state));
        return;
    }
    _state = State::GAME_STARTED;
}

void Client::handleTakeCards(Event::TakeCards* takeCards)
{
    LOG("TakeCards: numberOfCards=%d", takeCards->numberOfCards);
    for(int i{0}; i < takeCards->numberOfCards; ++i)
    {
        printf("Card: cardId=%d, cardType=%d\n", takeCards->cards[i].cardId, takeCards->cards[i].cardType);
        _player.addCard(takeCards->cards[i]);
    }
}

void Client::handleStartTurn(Event::StartTurn* startTurn)
{
    LOG("StartTurn: %d", startTurn->turnId);
    _turnId = startTurn->turnId;
    _inTurn = true;
}

void Client::handleTurnOver()
{
    LOG("TurnOver:");
    _inTurn = false;
}

void Client::handleEvents()
{
    Event::Header header;
    while(pullEvent(header))
    {
        switch(header.type)
        {
            case Event::GAME_INFO:
                handleGameInfo((Event::GameInfo*)header.data);
                break;
            case Event::START:
                handleStart();
                break;
            case Event::TAKE_CARDS:
                handleTakeCards((Event::TakeCards*)header.data);
                break;
            case Event::START_TURN:
                handleStartTurn((Event::StartTurn*)header.data);
                break;
            case Event::TURN_OVER:
                handleTurnOver();
                break;
            case Event::EXIT:
                handleExit((Event::Exit*)header.data);
                break;
        }
    }
}

void Client::joinGame()
{
    Event::Join join{};
    pushEvent(Event::JOIN, &join);
}

void Client::update(float dt)
{
    Scheduler::update(dt);
}