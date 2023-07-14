#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include "event.h"
#include "io.h"

class IEvent
{
public:

    IEvent(IO* io);
    virtual ~IEvent() noexcept;
    
    void startPoll();

    virtual bool pullEvent(Event::Header& header);

    void pushEvent(uint8_t eventType, void* data);

    void close();

    IO* io() const
    {
        return _io;
    }

private:
    IO* _io;
    std::queue<Event::Header> _eventQueue;
    std::thread _ioThread;
    std::mutex _queueMutex;
    bool _polling{false};
};