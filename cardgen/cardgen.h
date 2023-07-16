#pragma once

#include <iostream>
#include <filesystem>
#include "glapplication.h"
#include "glmesh.h"
#include "glcanvas.h"
#include "glfont.h"
#include "card.h"

class CardGen : public lithium::Application
{
public:
    CardGen();

    virtual ~CardGen() noexcept;

    virtual void update(float dt) override;

    virtual void onWindowSizeChanged(int width, int height) override;

    virtual void onFpsCount(int fps) override
    {
        std::cout << "FPS: " << fps << std::endl;
    }

    void generateImage();
    
    void loadCards();
    void loadCard(const std::filesystem::path& filename);

    void updateCard();

private:
    std::vector<std::shared_ptr<lithium::Object>> _objects;
    float _cameraAngle{0.0f};
    std::shared_ptr<lithium::Input::KeyCache> _keyCache;
    std::shared_ptr<lithium::Canvas> _canvas;
    glm::vec2 _dragPosition{0.0f};
    std::shared_ptr<lithium::Font> _font;
    std::vector<Card> _cards;
    std::vector<Card>::iterator _cardIt;
    std::shared_ptr<lithium::ImageTexture> _defaultTexture;
};
