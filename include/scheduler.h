#pragma once

#include <vector>
#include <functional>

class Scheduler
{
    struct Delay
    {
        std::function<bool(Scheduler*)> callback;
        float delay;
        float remaining;
    };

public:
    float time() const
    {
        return _time;
    }

    virtual void update(float dt)
    {
        _time += dt;
        auto it = _delayed.begin();
        while(it != _delayed.end())
        {
            it->remaining -= dt;
            if(it->remaining <= 0.0f)
            {
                if(it->callback(this) == false)
                {
                    it = _delayed.erase(it);
                }
                else
                {
                    it->remaining = it->delay;
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    void startDelayed(std::function<bool(Scheduler*)> callback, float delay)
    {
        Delay d;
        d.callback = callback;
        d.delay = delay;
        d.remaining = delay;
        _delayed.push_back(d);
    }

private:
    float _time{0.0f};
    std::vector<Delay> _delayed;
};