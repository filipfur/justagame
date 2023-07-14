#pragma once

#include <map>
#include <set>
#include "ievent.h"
#include "tile.h"
#include "io.h"
#include "scheduler.h"
#include "goptions.h"

class Game : public Scheduler
{
    enum class State
    {
        WAITING_FOR_SETTING,
        WAITING_FOR_PLAYERS,
        GAME_STARTED
    };

public:
    Game();
    ~Game() noexcept;

    virtual void update(float dt) override;

    void handleEvents();

    void handleSettings(Event::Settings* settings);
    void handleJoin(uint8_t clientId, Event::Join* join);
    void handleLeave(uint8_t clientId, Event::Leave* leave);
    void handleReady(uint8_t clientId);
    void handlePlayCard(uint8_t clientId, Event::PlayCard* playCard);

    void exit();

    void close();

    void addConnection(IO* io);

    Event::Card spawnCard();

    std::shared_ptr<IEvent> clientInTurn() const
    {
        return _clients.at(_turnId % _settings.numberOfPlayers);
    }

private:
    void sendToAll(uint8_t type, void* data)
    {
        for(auto& client : _clients)
        {
            client->pushEvent(type, data);
        }
    }

    void startTurn();

    std::vector<std::shared_ptr<IEvent>> _clients;
    std::vector<std::vector<Tile>> _tileMap;
    Event::Settings _settings;
    std::set<int> _ready;
    uint32_t _turnId{0};
    int _joined{0};
    uint64_t _turnOverId;
    std::map<int, Event::Card> _cards;
    State _state{State::WAITING_FOR_SETTING};
};