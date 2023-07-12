#pragma once

#include <stdint.h>
#include <assert.h>

namespace Event
{
    enum Type : uint8_t
    {
        NONE,
        JOIN,
        QUIT
    };

    struct Header
    {
        uint8_t eventId;
        uint8_t type;
        uint8_t data[1024];
    };

    struct Join
    {
        uint8_t clientId;
    };
    static_assert(sizeof(Join) <= 1024, "Event::Join is too large");

    struct Quit
    {
        uint8_t clientId;
    };
    static_assert(sizeof(Quit) <= 1024, "Event::Quit is too large");
}
