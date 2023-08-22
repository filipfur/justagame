#pragma once

#include "globject.h"
#include "ihand.h"

class CardObject : public lithium::Object
{
public:

    enum class State {
        Idle,
        Draw,
        InHand,
        Played,
        Discard,
        Gone
    };

    static std::shared_ptr<lithium::Mesh> sharedMesh;
    static std::vector<lithium::Object::TexturePointer> sharedTextures;

    CardObject(uint32_t id, uint32_t type);

    virtual ~CardObject() noexcept;

    virtual void update(float dt) override;

    virtual void shade(lithium::ShaderProgram* shaderProgram) override;

    uint32_t id() const
    {
        return _id;
    }

    uint32_t type() const
    {
        return _type;
    }

    void draw()
    {
        assert(_state == State::Idle);
        _state = State::Draw;
    }

    void play()
    {
        assert(_state == State::InHand);
        _state = State::Played;
    }

    void setIHand(IHand* iHand)
    {
        _iHand = iHand;
    }

    IHand* iHand() const
    {
        return _iHand;
    }

    bool isIdle() const
    {   
        return _state == State::Idle;
    }

private:
    IHand* _iHand{nullptr};
    uint32_t _id;
    uint32_t _type;
    State _state{State::Idle};
    glm::mat3 _cardData{0.0f};
    glm::vec3 _localStrive{0.0f, 0.0f, 0.0f};
    float _localR{0.0f};
};