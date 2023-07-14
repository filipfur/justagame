#pragma once

#include <functional>
#include "clientcontroller.h"

class TestController : public ClientController
{
public:
    TestController(Client* client) : ClientController{client}
    {

    }

    virtual ~TestController() noexcept
    {

    }

    void setOnNewTurnCallback(std::function<void(TestController*,uint32_t)> onNewTurn)
    {
        _onNewTurn = onNewTurn;
    }

    virtual void update(float dt) override
    {
        ClientController::update(dt);
        if(client()->state() == Client::State::GAME_STARTED && client()->inTurn())
        {
            if(_lastTurnId != client()->turnId())
            {
                _lastTurnId = client()->turnId();
                if(_onNewTurn)
                {
                    _onNewTurn(this, _lastTurnId);
                }
            }
        }
    }
private:
    uint32_t _lastTurnId{0xFFFFFFFF};
    std::function<void(TestController*,uint32_t)> _onNewTurn;
}; 