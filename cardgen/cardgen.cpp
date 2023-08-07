#include "cardgen.h"

#include <fstream>
#include "stb_image_write.h"

#include "glplane.h"
#include "utility.h"
#include "glcolor.h"

void CardGen::generateImage(const std::string& id)
{
    lithium::Frame* frame = _canvas->frameById(id);

    int width  = static_cast<int>(frame->dimension().x);
    int height = static_cast<int>(frame->dimension().y);
    GLubyte* data = new GLubyte[3 * width * height];
    memset(data, 0, 3 * width * height);

    frame->frameBuffer()->bind();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Write the PNG image
    int numOfComponents = 3; // RGB
    int strideInBytes   = width * 3;
    stbi_flip_vertically_on_write(true);
    stbi_write_png("test.png", width, height, 3, data, width * 3);
    stbi_image_free(data);
}

void CardGen::loadCards()
{
    std::filesystem::path cardDir{"cardgen/assets/cards"};
    for (const auto& entry : std::filesystem::directory_iterator(cardDir))
    {
        if (entry.is_directory())
        {
            loadCard(entry.path());
        }
    }
}

void parseLabel(std::ifstream& ifs, Card& card, const std::string& label)
{
    if(label == "@title")
    {
        std::string str;
        getline(ifs, str);
        card.title = str;
    }
}

void CardGen::parseCardFile(const std::filesystem::path& filename, Card& card)
{
    std::ifstream ifs{filename};
    std::string line;
    
    char c = ifs.peek();
    std::string str;
    while(c != EOF)
    {
        switch(c)
        {
            case '@':
                ifs >> str;
                parseLabel(ifs, card, str);
                break;
            default:
                getline(ifs, str);
                card.body += str + '\n';
                break;
        }
        c = ifs.peek();
    }
}

void CardGen::loadCard(const std::filesystem::path& directory)
{
    std::string name = directory.filename().string();
    std::filesystem::path textureFile = directory / (name + ".png");

    Card card;

    if(std::filesystem::exists(textureFile))
    {
        card.texture.reset((lithium::ImageTexture*)lithium::ImageTexture::load(textureFile.string(), GL_RGBA, GL_RGBA, 1, true));
        //card.texture->watch();
    }
    else
    {
        card.texture = _defaultTexture;
    }

    const auto cardFile = directory / (name + ".txt");
    card.path = cardFile;
    parseCardFile(cardFile, card);

    _cards.push_back(card);

    lithium::FileWatch::start(cardFile, [this](const std::filesystem::path& path){
        for(auto it = _cards.begin(); it != _cards.end(); ++it)
        {
            if(it->path == path)
            {
                it->body = "";
                parseCardFile(path, *it);
                if(_cardIt == it)
                {
                    updateCard();
                }
                break;
            }
        }
    });

}

void CardGen::updateCard()
{
    /*auto imageFrame = _canvas->frameById("canvas.0.0");
    imageFrame->setTextures(std::vector<lithium::Object::TexturePointer>{_cardIt->texture});

    auto titleFrame = _canvas->frameById("canvas.0.1");
    titleFrame->text()->setText(_cardIt->title);

    auto bodyFrame = _canvas->frameById("body");
    bodyFrame->text()->setText(_cardIt->body);

    imageFrame->refresh();
    titleFrame->refresh();
    bodyFrame->refresh();*/

    std::cout << "Selected card: " << _cardIt->title << std::endl;
}

CardGen::CardGen() : lithium::Application{"lithium-lab", glm::ivec2{1920, 1080}, lithium::Application::Mode::MULTISAMPLED_4X, false},
    _cardLayoutSystem{}
{
    _keyCache = std::make_shared<lithium::Input::KeyCache>(
        std::initializer_list<int>{GLFW_KEY_LEFT, GLFW_KEY_RIGHT});
    input()->setKeyCache(_keyCache);

    input()->addPressedCallback(GLFW_KEY_ESCAPE, [this](int key, int mods) {
        this->close();
        return true;
    });

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::shared_ptr<lithium::ImageTexture> fontTexture{lithium::ImageTexture::load("cardgen/assets/righteous/Righteous.png", GL_RGB, GL_RGBA, 1, false)};
    _font = std::make_shared<lithium::Font>(fontTexture, "cardgen/assets/righteous/Righteous.json");

    std::shared_ptr<lithium::ImageTexture> checkboardTexture{(lithium::ImageTexture*)lithium::ImageTexture::load("cardgen/assets/checkboard.png", GL_RGBA, GL_RGBA)
        ->setFilter(GL_NEAREST)->setWrap(GL_REPEAT)};

    //std::shared_ptr<lithium::ImageTexture> sunnyTexture{lithium::ImageTexture::load("cardgen/assets/cards/sunny/sunny.png", GL_RGBA, GL_RGBA, 1, true)};
    //std::shared_ptr<lithium::ImageTexture> bannerTexture{lithium::ImageTexture::load("cardgen/assets/banner.png", GL_RGBA, GL_RGBA, 1, true)};

    _defaultTexture.reset((lithium::ImageTexture*)lithium::ImageTexture::load("cardgen/assets/cards/default.png", GL_RGBA, GL_RGBA, 1, true));

    loadCards();

    std::cout << "Loaded " << _cards.size() << " cards" << std::endl;

    _cardIt = _cards.begin();

    auto res = windowResolution();

    glm::vec2 canvasDim{5400.0f};

    setMaxFps(60);

    _canvas = std::make_shared<lithium::Canvas>(res, defaultFrameBufferResolution());
    _canvas->loadUI("cardgen/assets/ui.json");

    auto canvasFrame = _canvas->frameById("canvas");
    canvasFrame->setMesh(std::shared_ptr<lithium::Mesh>(lithium::Plane2D(glm::vec2{0.5f}, glm::vec2{180.0f})));
    canvasFrame->setTextures(std::vector<lithium::Object::TexturePointer>{checkboardTexture});
    /*auto classFrame = _canvas->frameById("canvas.0");
    classFrame->setColor(glm::vec3{0.2f});

    auto imageFrame = _canvas->frameById("canvas.0.0");
    imageFrame->setTextures(std::vector<lithium::Object::TexturePointer>{_cardIt->texture});

    auto titleFrame = _canvas->frameById("canvas.0.1");
    titleFrame->setColor(glm::vec3{0.2f});

    auto myText = titleFrame->createTextRenderer()->createText(_font, _cardIt->title, 1.4f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);
    auto bodyFrame = _canvas->frameById("canvas.0.2");
    bodyFrame->setColor(glm::vec3{0.1});

    auto bodyText = bodyFrame->createTextRenderer()->createText(_font, _cardIt->body, 1.1f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);*/

    lithium::json::Json json;
    json.fromFile("cardgen/assets/card_a.json");
    _cardLayoutSystem.load(json);

    for(int i{0}; i < _cards.size(); ++i)
    {
        Card* card = &_cards[i];
        int n = i + 1;
        int x = n % 9;
        int y = n / 9;
        _cardLayoutSystem.root()->setPosition(glm::vec2{-2400.0f + x * 600.0f, 2250.0f + y * -900.0f});
        lithium::FrameLayout* cardLayout = canvasFrame->layout()->appendChild(*_cardLayoutSystem.root());
        _canvas->refreshUI();
        auto cardFrame = _canvas->addFrame(cardLayout);
        cardFrame->frameById("thumbnail")->setTextures(std::vector<lithium::Object::TexturePointer>{card->texture});
        dynamic_cast<lithium::Frame*>(cardFrame->frameById("title")->setColor(lithium::Color(0x222222)))
            ->createTextRenderer()->createText(_font, card->title, 1.4f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);
        dynamic_cast<lithium::Frame*>(cardFrame->frameById("body")->setColor(lithium::Color(0x111111)))
            ->createTextRenderer()->createText(_font, card->body, 1.1f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);
    }

    /*auto slot0Text = _canvas->frameById("slots.0.0");
    slot0Text->setColor(lithium::black);
    slot0Text->setOpacity(0.0f);
    slot0Text->createTextRenderer()->createText(_font, "1", 1.0f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);
    auto slot0Img = _canvas->frameById("slots.0");
    slot0Img->setTextures(std::vector<lithium::Object::TexturePointer>{checkboardTexture});*/

    //std::shared_ptr<lithium::Frame> slot1 = std::make_shared<lithium::Frame>(slot0Img->layout()->clone());
    //_canvas->refreshUI();

    /*auto slots = _canvas->frameById("slots");
    slots->setColor(glm::vec3{0.1f});*/

    /*auto classFrame2 = _canvas->frameById("canvas.1");
    classFrame2->setColor(glm::vec3{0.2f});
    classFrame2->child(0)->setTextures(std::vector<lithium::Object::TexturePointer>{_cardIt->texture});
    classFrame2->child(1)->setColor(glm::vec3{0.2f});
    classFrame2->child(1)->createTextRenderer()->createText(_font, _cardIt->title, 1.4f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);
    classFrame2->child(2)->setColor(glm::vec3{0.1f});
    classFrame2->child(2)->createTextRenderer()->createText(_font, _cardIt->body, 1.1f, lithium::Text::Alignment::CENTER, 1.0f, 1.05f);*/

    input()->setDragCallback([this](int button, int modifiers, const glm::vec2& start, const glm::vec2& current, const glm::vec2& delta, bool completed) {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            _canvas->move(glm::vec2(-delta.x, delta.y));
        }
    });

    input()->addPressedCallback(GLFW_KEY_ESCAPE, [this](int key, int mods) {
        this->close();
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_F1, [this](int key, int mods) {
        generateImage("canvas");
        return true;
    });
    input()->addPressedCallback(GLFW_KEY_F5, [this](int key, int mods) {
        generateImage("canvas.0");
        return true;
    });
    input()->addPressedCallback(GLFW_KEY_F6, [this](int key, int mods) {
        generateImage("canvas.1");
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_LEFT, [this](int key, int mods) {
        if(_cardIt != _cards.begin())
        {
            _cardIt--;
        }
        else
        {
            _cardIt = _cards.end() - 1;
        }
        updateCard();
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_RIGHT, [this](int key, int mods) {
        if(_cardIt != _cards.end() - 1)
        {
            _cardIt++;
        }
        else
        {
            _cardIt = _cards.begin();
        }
        updateCard();
        return true;
    });

    printf("%s\n", glGetString(GL_VERSION));
}

CardGen::~CardGen() noexcept
{
    _canvas = nullptr;
    _objects.clear();
}

void CardGen::update(float dt)
{
    for(auto o : _objects)
    {
        o->update(dt);
        o->setRotation(o->rotation() + glm::vec3{8.0f * dt});
    }

    if(_keyCache->isPressed(GLFW_KEY_LEFT))
    {
        _cameraAngle -= glm::pi<float>() * 0.5f * dt;
    }
    else if(_keyCache->isPressed(GLFW_KEY_RIGHT))
    {
        _cameraAngle += glm::pi<float>() * 0.5f * dt;
    }

    static const float cameraRadius = 6.0f;
    float camX = cos(_cameraAngle) * cameraRadius;
    static const float camY = cameraRadius * 0.5f;
    float camZ = sin(_cameraAngle) * cameraRadius;

    _canvas->update(dt);

    /*
    Wiggle canvas!
    static float r{0.0f};
    static int currS{0};
    int iTime = static_cast<int>(time());
    if(iTime != currS)
    {
        currS = iTime;
        r = utility::randn(-glm::pi<float>(), glm::pi<float>());
    }
    glm::vec2 v = glm::vec2{cos(r), sin(r)};
    _canvas->move(v * 64.0f * dt);*/

    _canvas->render();

    static int cntr{0};
    if(cntr < 11)
    {
        ++cntr;
    }
    if(cntr == 10)
    {
        //generateImage();
        std::cout << "Image generated!" << std::endl;
    }
}

void CardGen::onWindowSizeChanged(int width, int height)
{

}