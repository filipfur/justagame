#pragma once

#include <map>
#include "event.h"

class Player
{
public:
    Player()
    {

    }

    virtual ~Player() noexcept
    {

    }

    Event::Card spawnCard()
    {
        static int nextCardId{0};
        Event::Card card;
        card.cardId = nextCardId++;
        card.cardType = rand() % 10;
        _cards[card.cardId] = card;
        return card;
    }

    void addCard(Event::Card card)
    {
        _cards[card.cardId] = card;
    }

    void removeCard(int cardId)
    {
        _cards.erase(cardId);
    }

    bool hasCard(int cardId) const
    {
        return _cards.find(cardId) != _cards.end();
    }

    Event::Card card(int cardId) const
    {
        return _cards.at(cardId);
    }

    Event::Card cardByIndex(int index) const
    {
        auto it{_cards.begin()};
        std::advance(it, index);
        return it->second;
    }

    int cardCount() const
    {
        return _cards.size();
    }

private:
    std::map<int, Event::Card> _cards; // by cardId
};