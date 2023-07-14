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
    Event::GameInfo gameInfo;
    gameInfo.clientId = clientId;
    gameInfo.settings = _settings;
    static Event::i8vec2 startPos[] = {
        {static_cast<uint8_t>(_settings.mapSize.x / 2), 0},
        {static_cast<uint8_t>(_settings.mapSize.x / 2), static_cast<uint8_t>(_settings.mapSize.y - 1)},
        {0, static_cast<uint8_t>(_settings.mapSize.y / 2)},
        {static_cast<uint8_t>(_settings.mapSize.x - 1), static_cast<uint8_t>(_settings.mapSize.y / 2)}
    };
    gameInfo.clientId = clientId;
    gameInfo.position = startPos[clientId];
    _clients.at(clientId)->pushEvent(Event::GAME_INFO, &gameInfo);
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
        sendToAll(Event::START, nullptr);
        _state = State::GAME_STARTED;

        startTurn();
    }
}

void Game::handlePlayCard(uint8_t clientId, Event::PlayCard* playCard)
{
    LOG("PlayCard: client=%d cardId=%d x=%d y=%d", clientId, playCard->cardId, playCard->position.x, playCard->position.y);
    if(_state != State::GAME_STARTED)
    {
        printf("Warning: Wrong state: %d\n", static_cast<int>(_state));
        return;
    }

    if(clientId != _turnId % _settings.numberOfPlayers)
    {
        printf("Warning: Wrong turn: %d\n", clientId);
        return;
    }

    if(_cards.find(playCard->cardId) == _cards.end()) // Card map should be per client along with other values, oil, cash, etc, etc.
    {
        printf("Warning: Unknown card: %d\n", playCard->cardId);
        return;
    }
}

Event::Card Game::spawnCard()
{
    static int nextCardId{0};
    Event::Card card;
    card.cardId = nextCardId++;
    card.cardType = rand() % 10;
    _cards[card.cardId] = card;
    return card;
}

void Game::startTurn()
{
    startDelayed([this](Scheduler* scheduler) -> bool
    {
        Event::Card card = spawnCard();
        clientInTurn()->pushEvent(Event::CARD, &card);
        return false;
    }, 0.1f);

    startDelayed([this](Scheduler* scheduler) -> bool
    {
        Event::StartTurn startTurn{_turnId};
        clientInTurn()->pushEvent(Event::START_TURN, &startTurn);
        return false;
    }, goptions::cardToStartTurnDelay);

    _turnOverId = startDelayed([this](Scheduler* scheduler) -> bool
    {
        clientInTurn()->pushEvent(Event::TURN_OVER, nullptr);
        return false;
    }, goptions::cardToStartTurnDelay + goptions::turnTime);
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
            case Event::PLAY_CARD:
                handlePlayCard(clientId, (Event::PlayCard*)event.data);
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