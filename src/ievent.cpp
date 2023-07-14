#include "ievent.h"

#include <iostream>

IEvent::IEvent(IO* io) : _io{io}
{

}

IEvent::~IEvent() noexcept
{
    _io = nullptr;
}

void IEvent::startPoll()
{
    _polling = true;
    _ioThread = std::thread([this](){
        while(_polling)
        {
            Event::Header header;
            if(_io->read((char*)&header) > 0)
            {
                std::lock_guard<std::mutex> lk(_queueMutex);
                _eventQueue.push(header);
            }
        }
    });
}

void IEvent::close()
{
    _polling = false;
    _io->disconnect();
    _ioThread.join();
}

bool IEvent::pullEvent(Event::Header& header)
{
    std::lock_guard<std::mutex> lk(_queueMutex);
    if(_eventQueue.empty())
    {
        return false;
    }
    header = _eventQueue.front();
    _eventQueue.pop();
    //std::cout << "Event: " << (int)header.eventId << ", type=" << (int)header.type << "\n";
    return true;
}

void IEvent::pushEvent(uint8_t eventType, void* data)
{
    Event::Header header{0x0, eventType};
    if(data != nullptr)
    {
        memcpy(header.data, data, sizeof(header.data));
    }
    if(!_io->isConnected())
    {
        throw std::runtime_error("IO not connected");
    }
    _io->write((char*)&header, sizeof(Event::Header));
}