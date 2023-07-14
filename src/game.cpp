#include <string>
#include "game.h"

#define LOG(str, ...) printf("%8.3f [server] " str "\n", time(), ##__VA_ARGS__)

Game::Game()
{

}

Game::~Game() noexcept
{
}

void Game::handleSettings(Event::Settings* settings)
{
    LOG("Settings: players=%d, gridX=%d, gridY=%d",
        settings->numberOfPlayers,
        settings->mapSize.x,
        settings->mapSize.y);
    _settings = *settings;
    _state = State::WAITING_FOR_PLAYERS;
    _tileMap.resize(settings->mapSize.y);
    for(int y{0}; y < settings->mapSize.y; ++y)
    {
        _tileMap[y].resize(settings->mapSize.x);
    }
    printf("Tiles: cols=%lu, rows=%lu size=%d\n",
        _tileMap.size(),
        _tileMap[0].size(),
        static_cast<int>(sizeof(Tile) * _tileMap.size() * _tileMap[0].size()));
    
}

void Game::handleJoin(uint8_t clientId, Event::Join* join)
{
    LOG("Join: %d", clientId);
    if(_state != State::WAITING_FOR_PLAYERS)
    {
        printf("Warning: Wrong state: %d", static_cast<int>(_state));
        return;
    }
    Event::Identity identity{clientId};
    _clients.at(clientId)->pushEvent(Event::IDENTITY, &identity);
    ++_joined;
}

void Game::handleLeave(uint8_t clientId, Event::Leave* leave)
{
    LOG("Leave: %d", leave->clientId);
}

void Game::handleReady(uint8_t clientId)
{
    LOG("Ready: %d", clientId);
    if(_state != State::WAITING_FOR_PLAYERS)
    {
        printf("Warning: Wrong state: %d\n", static_cast<int>(_state));
        return;
    }
    _ready.insert(clientId);
    if(_ready.size() == _settings.numberOfPlayers)
    {
        Event::Start start;
        start.settings = _settings;
        static Event::i8vec2 startPos[] = {
            {static_cast<uint8_t>(_settings.mapSize.x / 2), 0},
            {static_cast<uint8_t>(_settings.mapSize.x / 2), static_cast<uint8_t>(_settings.mapSize.y - 1)},
            {0, static_cast<uint8_t>(_settings.mapSize.y / 2)},
            {static_cast<uint8_t>(_settings.mapSize.x - 1), static_cast<uint8_t>(_settings.mapSize.y / 2)}
        };
        for(int i{0}; i < _clients.size(); ++i)
        {
            start.position = startPos[i];
            _clients.at(i)->pushEvent(Event::START, &start);
        }
        _state = State::GAME_STARTED;
    }
}

void Game::handleEvents()
{
    Event::Header event;
    for(uint8_t clientId{0}; clientId < _clients.size(); ++clientId)
    {
        auto& client = _clients.at(clientId);
        while(client->pullEvent(event))
        {
            switch(event.type)
            {
            case Event::SETTINGS:
                handleSettings((Event::Settings*)event.data);
                break;
            case Event::JOIN:
                handleJoin(clientId, (Event::Join*)event.data);
                break;
            case Event::LEAVE:
                handleLeave(clientId, (Event::Leave*)event.data);
                break;
            case Event::READY:
                handleReady(clientId);
                break;
            default:
                throw std::runtime_error("unknown event type: " + std::to_string(event.type));
            }
        }
    }
}

void Game::update(float dt)
{
    Scheduler::update(dt);
}

void Game::addConnection(IO* io)
{
    std::shared_ptr<IEvent> clientEvent = std::make_shared<IEvent>(io);
    clientEvent->startPoll();
    _clients.push_back(clientEvent);
}

void Game::exit()
{
    Event::Exit exit;
    for(auto& client : _clients)
    {
        client->pushEvent(Event::EXIT, &exit);
    }
}

void Game::close()
{
    for(auto& client : _clients)
    {
        client->close();
    }
}