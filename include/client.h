#pragma once

#include "ievent.h"
#include "scheduler.h"

class Client : public IEvent, public Scheduler
{
public:
    enum class State
    {
        WAITING_FOR_GAME_INFO,
        WAITING_FOR_START,
        GAME_STARTED
    };

    Client(IO* io);

    virtual ~Client() noexcept;


    void handleGameInfo(Event::GameInfo* start);
    void handleStart();
    void handleExit(Event::Exit* exit);
    void handleCard(Event::Card* card);
    void handleStartTurn(Event::StartTurn* startTurn);
    void handleTurnOver();

    void handleEvents();

    void joinGame();

    virtual void update(float dt) override;

    State state() const
    {
        return _state;
    }

    bool inTurn() const
    {
        return _inTurn;
    }

    uint32_t turnId() const
    {
        return _turnId;
    }

private:
    Event::GameInfo _gameInfo;
    State _state{State::WAITING_FOR_GAME_INFO};
    uint32_t _turnId{0};
    bool _inTurn{false};
};