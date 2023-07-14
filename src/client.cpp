#include "client.h"

#define LOG(str, ...) printf("%8.3f [client] " str "\n", time(), ##__VA_ARGS__)

Client::Client(IO* io) : IEvent{io}
{

}

Client::~Client() noexcept
{

}

void Client::handleIdentity(Event::Identity* identity)
{
    LOG("Identity: %d", identity->clientId);
    _identity = *identity;
    startDelayed([this](Scheduler* scheduler) -> bool
    {
        if(!_gameStarted)
        {
            pushEvent(Event::READY, nullptr);
        }
        return !_gameStarted;
    }, 0.1f);
}

void Client::handleExit(Event::Exit* exit)
{
    LOG("Exit: %s", exit->message);
    close();
}

void Client::handleStart(Event::Start* start)
{
    LOG("Start: players=%d cols=%d rows=%d startX=%d startY=%d", start->settings.numberOfPlayers,
        start->settings.mapSize.x,
        start->settings.mapSize.y,
        start->position.x,
        start->position.y);
    _gameStarted = true;
}

void Client::handleEvents()
{
    Event::Header header;
    while(pullEvent(header))
    {
        switch(header.type)
        {
            case Event::IDENTITY:
                handleIdentity((Event::Identity*)header.data);
                break;
            case Event::EXIT:
                handleExit((Event::Exit*)header.data);
                break;
            case Event::START:
                handleStart((Event::Start*)header.data);
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