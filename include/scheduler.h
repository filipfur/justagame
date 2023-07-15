#pragma once

#include <vector>
#include <functional>

class Scheduler
{
    struct Delay
    {
        uint64_t id;
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
                assert(it->callback);
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

    uint64_t startDelayed(std::function<bool(Scheduler*)> callback, float delay)
    {
        Delay d;
        d.id = _nextId++;
        d.callback = callback;
        d.delay = delay;
        d.remaining = delay;
        _delayed.push_back(d);
        return d.id;
    }

    void stopDelayed(uint64_t id)
    {
        auto it = _delayed.begin();
        while(it != _delayed.end())
        {
            if(it->id == id)
            {
                it = _delayed.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }

private:
    float _time{0.0f};
    std::vector<Delay> _delayed;
    uint64_t _nextId{0};
};