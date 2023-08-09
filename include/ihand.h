#pragma once

class CardObject;

class IHand
{
public:
    virtual int slots() const = 0;
    virtual int cards() const = 0;
    virtual int index(CardObject* cardObject) const = 0;
};