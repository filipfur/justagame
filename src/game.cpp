#include "game.h"

Game::Game()
{
}

Game::~Game() noexcept
{
}

void Game::handleJoin(Event::Join* join)
{
    printf("Join: %d\n", join->clientId);
}

void Game::handleQuit(Event::Quit* quit)
{
    printf("Quit: %d\n", quit->clientId);
}

void Game::handleEvents()
{
    while(!_events.empty())
    {
        Event::Header& event = _events.front();
        switch(event.type)
        {
        case Event::JOIN:
            handleJoin((Event::Join*)event.data);
            break;
        case Event::QUIT:
            handleQuit((Event::Quit*)event.data);
            break;
        }
        _events.pop();
    }
}