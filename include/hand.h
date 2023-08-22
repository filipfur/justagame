#pragma once

#include <set>
#include "ihand.h"
#include "cardobject.h"
#include "glupdateable.h"

class Hand : public IHand, public lithium::Updateable
{
public:
    Hand()
    {
        _hovered = _cards.end();
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
        if(!_cardsToRemove.empty())
        {
            for(CardObject* cardObject : _cardsToRemove)
            {
                for(auto it = _cards.begin(); it != _cards.end(); ++it)
                {
                    if(it->get() == cardObject)
                    {
                        _cards.erase(it);
                        break;
                    }
                }
            }
            _cardsToRemove.clear();
            _hovered = _cards.end();
        }
    }

    void addCard(std::shared_ptr<CardObject> card)
    {
        _cards.push_back(card);
        card->setIHand(this);
        _hovered = _cards.end();
    }

    virtual int slots() const override
    {
        return 7;
    }

    virtual int cards() const override
    {
        return _cards.size();
    }

    virtual void onPlayed(CardObject* cardObject) override
    {
        removeCard(cardObject);
    }

    CardObject* card(size_t index) const
    {
        return _cards[index].get();
    }

    CardObject* cardById(int id) const
    {
        for(auto& card : _cards)
        {
            if(card->id() == id)
            {
                return card.get();
            }
        }
        return nullptr;
    }

    CardObject* randomCard() const
    {
        return _cards[rand() % _cards.size()].get();
    }

    CardObject* hovered() const
    {
        return _hovered == _cards.end() ? nullptr : _hovered->get();
    }

    bool isHovered(CardObject* cardObject) const override
    {
        return _hovered != _cards.end() && _hovered->get() == cardObject;
    }

    void removeCard(CardObject* cardObject)
    {
        _cardsToRemove.insert(cardObject);
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

    void cycleHover()
    {
        if(_hovered == _cards.end())
        {
            _hovered = _cards.begin();
        }
        else
        {
            ++_hovered;
            if(_hovered == _cards.end())
            {
                _hovered = _cards.begin();
            }
        }
    }

    void hover(int id)
    {
        for(auto it = _cards.begin(); it != _cards.end(); ++it)
        {
            if((*it)->id() == id)
            {
                _hovered = it;
                return;
            }
        }
        _hovered = _cards.end();
    }

private:
    std::vector<std::shared_ptr<CardObject>> _cards;
    std::set<CardObject*> _cardsToRemove;
    std::vector<std::shared_ptr<CardObject>>::iterator _hovered;
};