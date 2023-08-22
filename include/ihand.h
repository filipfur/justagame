#pragma once

class CardObject;

class IHand
{
public:
    virtual int slots() const = 0;
    virtual int cards() const = 0;
    virtual int index(CardObject* cardObject) const = 0;
    virtual bool isHovered(CardObject* cardObject) const = 0;
    virtual void onPlayed(CardObject* cardObject) = 0;
};