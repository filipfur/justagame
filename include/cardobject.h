#pragma once

#include "globject.h"
#include "ihand.h"

class CardObject : public lithium::Object
{
public:
    static std::shared_ptr<lithium::Mesh> sharedMesh;
    static std::vector<lithium::Object::TexturePointer> sharedTextures;

    CardObject(uint32_t id, uint32_t type);

    virtual ~CardObject() noexcept;

    virtual void update(float dt) override;

    virtual void shade(lithium::ShaderProgram* shaderProgram) override;

    void setIHand(IHand* iHand)
    {
        _iHand = iHand;
    }

    IHand* iHand() const
    {
        return _iHand;
    }

private:
    IHand* _iHand{nullptr};
    uint32_t _id;
    uint32_t _type;
    glm::vec4 _cardData;
};