#include "app.h"

#include "assetfactory.h"

App::App() : Application{"lithium-lab", glm::ivec2{1440, 800}, lithium::Application::Mode::MULTISAMPLED_4X, false}
{
    // Load all assets from the filesystem.
    AssetFactory::loadMeshes();
    AssetFactory::loadTextures();
    AssetFactory::loadObjects();
    AssetFactory::loadFonts();
    
    // Create the render pipeline
    _pipeline = std::make_shared<Pipeline>(defaultFrameBufferResolution());

    // Create and add a background plane to the render pipeline, and stage it for rendering.
    _background = std::make_shared<lithium::Object>(AssetFactory::getMeshes()->screen,
        std::vector<lithium::Object::TexturePointer>{});
    _background->setGroupId(Pipeline::BACKGROUND);
    _pipeline->attach(_background.get());
    _background->stage();

    // Create and add a cube to the render pipeline, and stage it for rendering.
    auto card = std::make_shared<lithium::Object>(AssetFactory::getMeshes()->card,
        std::vector<lithium::Object::TexturePointer>{AssetFactory::getTextures()->card});
    card->setPosition(glm::vec3{0.0f});
    card->setScale(1.0f);
    _pipeline->attach(card.get());
    _objects.push_back(card);
    card->stage();

    // Key cache for rotating the camera left and right.
    _keyCache = std::make_shared<lithium::Input::KeyCache>(
        std::initializer_list<int>{GLFW_KEY_LEFT, GLFW_KEY_RIGHT});
    input()->setKeyCache(_keyCache);

    // Escape key to close the application.
    input()->addPressedCallback(GLFW_KEY_ESCAPE, [this](int key, int mods) {
        this->close();
        return true;
    });

    // Set the camera oirigin position and target.
    _pipeline->camera()->setPosition(glm::vec3{0.0f, 0.0f, 3.0f});
    _pipeline->camera()->setTarget(glm::vec3{0.0f});

    printf("%s\n", glGetString(GL_VERSION));
}

App::~App() noexcept
{
    _pipeline = nullptr;
    _background = nullptr;
    _objects.clear();
}

void App::update(float dt)
{
    // Apply a rotation to the cube.
    /*for(auto o : _objects)
    {
        o->update(dt);
        o->setRotation(glm::vec3{0.0f, 8.0f * sin(time()), 0.0f});
    }*/

    // Rotate the camera around the origin on player input.
    if(_keyCache->isPressed(GLFW_KEY_LEFT))
    {
        _cameraAngle -= glm::pi<float>() * 0.5f * dt;
    }
    else if(_keyCache->isPressed(GLFW_KEY_RIGHT))
    {
        _cameraAngle += glm::pi<float>() * 0.5f * dt;
    }
    static const float cameraRadius = 1.6f;
    float camX = sin(_cameraAngle) * cameraRadius;
    static const float camY = cameraRadius * 0.3f;
    float camZ = cos(_cameraAngle) * cameraRadius;
    _pipeline->camera()->setPosition(glm::vec3{camX, camY, camZ});
    _pipeline->render();
}

void App::onWindowSizeChanged(int width, int height)
{
    _pipeline->setResolution(glm::ivec2{width, height});
}