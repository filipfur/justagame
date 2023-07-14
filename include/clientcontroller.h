#pragma once

#include "client.h"

class ClientController : public Scheduler
{
public:
    ClientController(Client* client) : _client{client}
    {

    }

    virtual ~ClientController() noexcept
    {

    }

    virtual void update(float dt) override
    {
        Scheduler::update(dt);
        _client->update(dt);
    }

    Client* client() const
    {
        return _client;
    }

private:
    Client* _client;
};