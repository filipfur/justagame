#pragma once

#include "client.h"

class TestClient : public Client
{
public:
    TestClient(IO* io) : Client{io}
    {

    }

    virtual ~TestClient() noexcept
    {

    }

    virtual void update(float dt) override
    {
        Client::update(dt);
    }
};