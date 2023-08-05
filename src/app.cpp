#include "app.h"

#include "assetfactory.h"

#include "glcube.h"

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
    /*
    auto card = std::make_shared<lithium::Object>(AssetFactory::getMeshes()->card,
        std::vector<lithium::Object::TexturePointer>{AssetFactory::getTextures()->card});
    card->setPosition(glm::vec3{0.0f});
    card->setScale(1.0f);
    _pipeline->attach(card.get());
    _objects.push_back(card);
    card->stage();
    */

    static constexpr size_t numSpheres = 15;

    static glm::vec3 spherePositions[numSpheres] = {
        glm::vec3{-2.0f, 0.5f, 0.0f},
        glm::vec3{-1.0f, 0.5f, 0.0f},
        glm::vec3{0.0f, 0.5f, 0.0f},
        glm::vec3{1.0f, 0.5f, 0.0f},
        glm::vec3{2.0f, 0.5f, 0.0f},
        glm::vec3{-2.0f, -0.5f, 0.0f},
        glm::vec3{-1.0f, -0.5f, 0.0f},
        glm::vec3{0.0f, -0.5f, 0.0f},
        glm::vec3{1.0f, -0.5f, 0.0f},
        glm::vec3{2.0f, -0.5f, 0.0f},
        glm::vec3{-2.0f, -1.5f, 0.0f},
        glm::vec3{-1.0f, -1.5f, 0.0f},
        glm::vec3{0.0f, -1.5f, 0.0f},
        glm::vec3{1.0f, -1.5f, 0.0f},
        glm::vec3{2.0f, -1.5f, 0.0f}
    };

    static float sphereMetallics[numSpheres] = {
        0.01f,
        0.25f,
        0.5f,
        0.75f,
        0.99f,
        0.01f,
        0.25f,
        0.5f,
        0.75f,
        0.99f,
        0.01f,
        0.25f,
        0.5f,
        0.75f,
        0.99f
    };

    static float sphereRoughnesses[numSpheres] = {
        0.01f,
        0.01f,
        0.01f,
        0.01f,
        0.01f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.99f,
        0.99f,
        0.99f,
        0.99f,
        0.99f,
    };

    static glm::vec3 sphereColors[numSpheres] = {
        glm::vec3{1.0f, 0.0f, 0.0f},
        glm::vec3{0.0f, 1.0f, 0.0f},
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{0.0f, 0.0f, 1.0f},
        glm::vec3{1.0f, 1.0f, 0.0f},
    };

    for(int i{0}; i < numSpheres; ++i)
    {
        auto sphere = std::make_shared<lithium::Object>(
            std::shared_ptr<lithium::Mesh>(AssetFactory::getMeshes()->sphere->clone()),
            std::vector<lithium::Object::TexturePointer>{});
        sphere->setPosition(spherePositions[i]);
        sphere->setScale(glm::vec3{0.4f});
        sphere->setGroupId(Pipeline::PBR);
        sphere->mesh()->setMaterial(std::make_shared<lithium::Material>(
            glm::vec4{sphereColors[i % 5], 1.0f},
            sphereMetallics[i],
            sphereRoughnesses[i]));
        _objects.push_back(sphere);
        _pipeline->attach(sphere.get());
        sphere->stage();
    }

    auto sphere = std::make_shared<lithium::Object>(
            std::shared_ptr<lithium::Mesh>(AssetFactory::getMeshes()->sphere->clone()),
            AssetFactory::getTextures()->rustedIron);
    sphere->setPosition(glm::vec3{0.0f, 1.5f, 0.0f});
    sphere->setScale(glm::vec3{0.4f});
    sphere->setGroupId(Pipeline::PBR);
    _objects.push_back(sphere);
    _pipeline->attach(sphere.get());
    sphere->stage();

    auto goldSphere = std::make_shared<lithium::Object>(
            std::shared_ptr<lithium::Mesh>(AssetFactory::getMeshes()->sphere->clone()),
            AssetFactory::getTextures()->scuffedGold);
    goldSphere->setPosition(glm::vec3{1.0f, 1.5f, 0.0f});
    goldSphere->setScale(glm::vec3{0.4f});
    goldSphere->setGroupId(Pipeline::PBR);
    _objects.push_back(goldSphere);
    _pipeline->attach(goldSphere.get());
    goldSphere->stage();

    //_cubemapHDR = lithium::CubemapHDR::load("assets/belfast_sunset_puresky_16k.hdr");
    //_cubemapHDR = lithium::CubemapHDR::load("assets/abandoned_tiled_room_16k.hdr");
    _cubemapHDR = lithium::CubemapHDR::load("assets/chess.hdr");
    _cubemapHDR->brdfLUT()->bind(GL_TEXTURE7);
    _cubemapHDR->irradianceMap()->bind(GL_TEXTURE8);
    _cubemapHDR->prefilterMap()->bind(GL_TEXTURE9);


    _skybox = std::shared_ptr<lithium::Object>(new lithium::Object(std::shared_ptr<lithium::Mesh>(lithium::InvCube()), {
        /*AssetFactory::getTextures()->skybox*/}));
    _skybox->setGroupId(Pipeline::SKYBOX);
    _skybox->setShaderCallback([this](lithium::Renderable* r, lithium::ShaderProgram* shaderProgram) {
        shaderProgram->setUniform("u_lod", _lod);
    });

    _pipeline->attach(_skybox.get());
    _skybox->stage();

    /*for(auto o : AssetFactory::getObjects()->windTurbine)
    {
        _pipeline->attach(o.get());
        o->stage();
    }*/

    /*for(auto o : AssetFactory::getObjects()->barberChair)
    {
        o->setTextures(AssetFactory::getTextures()->barberChair);
        o->setGroupId(Pipeline::PBR_POLY_HAVEN);
        _pipeline->attach(o.get());
        o->stage();
        o->setScale(2.0f);
    }*/

    // Key cache for rotating the camera left and right.
    _keyCache = std::make_shared<lithium::Input::KeyCache>(
        std::initializer_list<int>{GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN});
    input()->setKeyCache(_keyCache);

    // Escape key to close the application.
    input()->addPressedCallback(GLFW_KEY_ESCAPE, [this](int key, int mods) {
        this->close();
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_KP_ADD, [this](int key, int mods) {
        _lod += 0.25f;
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_KP_SUBTRACT, [this](int key, int mods) {
        _lod -= 0.25f;
        _lod = std::max(0.0f, _lod);
        return true;
    });

    setMaxFps(60.0f);

    // Set the camera oirigin position and target.
    _pipeline->camera()->setTarget(glm::vec3{0.0f, 0.0f, 0.0f});

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
    if(_keyCache->isPressed(GLFW_KEY_UP))
    {
        _camY += 5.0f * dt;
    }
    else if(_keyCache->isPressed(GLFW_KEY_DOWN))
    {
        _camY -= 5.0f * dt;
    }
    static const float cameraRadius = 4.4f;
    float camX = sin(_cameraAngle) * cameraRadius;
    float camZ = cos(_cameraAngle) * cameraRadius;
    _pipeline->camera()->setPosition(glm::vec3{camX, _camY, camZ});
    _cubemapHDR->bind(GL_TEXTURE0);
    _pipeline->render();
    //lithium::CubemapHDR::draw(*_cubemapHDR, *_pipeline->camera());
}

void App::onWindowSizeChanged(int width, int height)
{
    _pipeline->setResolution(glm::ivec2{width, height});
}