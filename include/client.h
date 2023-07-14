#pragma once

#include "ievent.h"
#include "scheduler.h"

class Client : public IEvent, public Scheduler
{
public:
    Client(IO* io);

    virtual ~Client() noexcept;



    void handleIdentity(Event::Identity* identity);
    void handleExit(Event::Exit* exit);
    void handleStart(Event::Start* start);

    void handleEvents();

    void joinGame();

    virtual void update(float dt) override;

private:
    Event::Identity _identity;
    Event::Start _start;
    bool _gameStarted{false};
};