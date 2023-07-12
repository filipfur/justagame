#pragma once

#include "event.h"
#include <queue>

class Game
{
public:
    Game();
    ~Game() noexcept;

    void update(float dt);

    void pushEvent(uint8_t eventType, void* data)
    {
        Event::Header event{0xFF, eventType};
        memcpy(event.data, data, 1024);
        _events.push(event);
    }

    void handleEvents();

    void handleJoin(Event::Join* join);
    void handleQuit(Event::Quit* quit);

private:
    std::queue<Event::Header> _events;
};