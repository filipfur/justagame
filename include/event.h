#pragma once

#include <stdint.h>
#include <assert.h>
#include "goptions.h"

namespace Event
{
    enum Type : uint8_t
    {
        NONE,
        SETTINGS,
        JOIN,
        GAME_INFO,
        LEAVE,
        READY,
        START,

        TAKE_CARDS,
        START_TURN,
        PASS,
        TURN_OVER,

        PLAY_CARD,

        EXIT
    };

    struct Header
    {
        uint8_t eventId;
        uint8_t type;
        uint8_t data[1024];
    };

    struct i8vec2
    {
        uint8_t x;
        uint8_t y;
    };

    struct Card
    {
        uint8_t cardId;
        uint8_t cardType;
    };

    struct Settings
    {
        uint8_t numberOfPlayers;
        i8vec2 mapSize;
    };
    static_assert(sizeof(Settings) <= 1024, "Event::Settings is too large");

    /*
        From client to server.
    */
    struct Join
    {
    };
    static_assert(sizeof(Join) <= 1024, "Event::Join is too large");

    struct Leave
    {
        uint8_t clientId;
    };
    static_assert(sizeof(Leave) <= 1024, "Event::Leave is too large");

    struct PlayCard
    {
        uint8_t cardId;
        i8vec2 position;
    };

    /*
        From server to client.
    */
    struct PlayerState
    {
        int32_t oil;
        int32_t reputation;
        int32_t cash;
        int32_t knowledge;
    };

    struct GameInfo
    {
        uint8_t clientId;
        Settings settings;
        i8vec2 position;
        Card cards[goptions::startNumberOfCards];
    };

    struct StartTurn
    {
        uint32_t turnId;
    };

    struct TakeCards
    {
        uint8_t numberOfCards;
        Card cards[8];
    };

    struct Exit
    {
        char message[128] = "goodbye";
    };
    static_assert(sizeof(Exit) <= 1024, "Event::Exit is too large");
}
