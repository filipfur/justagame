#include "cardobject.h"

std::shared_ptr<lithium::Mesh> CardObject::sharedMesh{nullptr};
std::vector<lithium::Object::TexturePointer> CardObject::sharedTextures{};

CardObject::CardObject(uint32_t id, uint32_t type) : Object{sharedMesh, sharedTextures}, _id(id), _type(type)
{
    int n = type;
    assert(n > 0);
    int X = n % 9;
    int Y = n / 9;
    assert(Y < 6);
    _cardData = glm::vec4{
        X / 9.0f,
        Y / 6.0f,
        0.0f,
        -0.4f
    };
}

CardObject::~CardObject() noexcept
{
}

void CardObject::update(float dt)
{
    lithium::Object::update(dt);
    static const float cardSpacing{0.1f};
    _cardData.z = -_iHand->cards() * 0.5f * cardSpacing + _iHand->index(this) * cardSpacing;
}

void CardObject::shade(lithium::ShaderProgram* shaderProgram)
{
    lithium::Object::shade(shaderProgram);
    shaderProgram->setUniform("u_card_data", _cardData);
}