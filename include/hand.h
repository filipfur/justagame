#pragma once

#include "ihand.h"
#include "cardobject.h"
#include "glupdateable.h"

class Hand : public IHand, public lithium::Updateable
{
public:
    Hand()
    {

    }

    virtual ~Hand() noexcept
    {

    }

    virtual void update(float dt) override
    {
        lithium::Updateable::update(dt);
        for(auto& card : _cards)
        {
            card->update(dt);
        }
    }

    void addCard(std::shared_ptr<CardObject> card)
    {
        _cards.push_back(card);
        card->setIHand(this);
    }

    virtual int slots() const override
    {
        return 7;
    }

    virtual int cards() const override
    {
        return _cards.size();
    }

    virtual int index(CardObject* cardObject) const override
    {
        for(int idx{0}; idx < _cards.size(); ++idx)
        {
            if(_cards[idx].get() == cardObject)
            {
                return idx;
            }
        }
        return -1;
    }

private:
    std::vector<std::shared_ptr<CardObject>> _cards;
};