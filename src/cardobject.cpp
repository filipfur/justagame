#include "cardobject.h"

std::shared_ptr<lithium::Mesh> CardObject::sharedMesh{nullptr};
std::vector<lithium::Object::TexturePointer> CardObject::sharedTextures{};

static const float yStart{-0.6f};

CardObject::CardObject(uint32_t id, uint32_t type) : Object{sharedMesh, sharedTextures}, _id(id), _type(type)
{
    int n = type;
    assert(n > 0);
    std::cout << "n = " << n << "\n";
    int X = n % 9;
    int Y = n / 9;
    assert(Y < 6);
    _cardData = glm::mat3{
        X / 9.0f,
        -Y / 6.0f,
        0.0f,
        
        0.0f,
        yStart,
        0.0f,

        0.0f,
        0.0f,
        0.0f
    };
}

CardObject::~CardObject() noexcept
{
}

void CardObject::update(float dt)
{
    lithium::Object::update(dt);
    static const float cardSpacing{0.28f};

    int nCards = _iHand->cards();

    float stackWidth = (nCards - 1) * cardSpacing;


    float& r = _cardData[2][1];

    float fatness = (static_cast<float>(nCards) / _iHand->slots());

    _localStrive.x = -stackWidth * 0.5f + _iHand->index(this) * cardSpacing;

    if(_state == State::Played)
    {
        _localStrive.y = 0.0f;
        _localR = 0.0f;
        _localStrive.z = -0.4f;
    }
    else if(_iHand->isHovered(this))
    {
        _localStrive.y = -0.4f;
        _localR = 0.0f;
        _localStrive.z = 0.1;
    }
    else
    {
        float f{0.0f};
        if(nCards > 1)
        {
            // set _cardDraw.y to a value between -pi and pi based on the index of the card in the hand
            f = static_cast<float>(_iHand->index(this)) / (static_cast<float>(nCards - 1)) - 0.5f;
            _localR = f * glm::pi<float>() * 0.25f * fatness;
        }
        else
        {
            _localR = 0.0f;
        }
        _localStrive.y = yStart - glm::abs(f) * 0.4f * fatness;
        _localStrive.z = 0.0f;
    }
    

    glm::vec3& strive = _cardData[1];
    strive = glm::mix(strive, _localStrive, dt * 5.0f);
    r = glm::mix(r, _localR, dt * 5.0f);

    float& t = _cardData[2][0];

    switch(_state)
    {
        case State::Idle:
            t = 0.0f;
            break;
        case State::Draw:
            t += dt * 2.0f;
            if(t >= 1.0f)
            {
                t = 1.0f;
                _state = State::InHand;
            }
            break;
        case State::InHand:
            t = 1.0f;
            break;
        case State::Played:
            setOpacity(opacity() - dt * 2.0f);
            setColor(glm::vec3(color().x + dt * 20.0f));
            if(opacity() <= 0.0f)
            {
                setOpacity(0.0f);
                _iHand->onPlayed(this);
                _state = State::Gone;
            }
            break;
        default:
            t = 0.0f;
            break;
    }
    _cardData[2][0] = t;
}

void CardObject::shade(lithium::ShaderProgram* shaderProgram)
{
    lithium::Object::shade(shaderProgram);
    shaderProgram->setUniform("u_card_data", _cardData);
    shaderProgram->setUniform("u_id", static_cast<int>(_id));
}