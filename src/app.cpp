#include "app.h"

#include "assetfactory.h"

#include "glcube.h"
#include "cardobject.h"
#include "glinstancedobject.h"

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

    CardObject::sharedMesh = AssetFactory::getMeshes()->card;
    CardObject::sharedTextures.push_back(AssetFactory::getTextures()->card);

    // Create and add a cube to the render pipeline, and stage it for rendering.
    for(int i{0}; i < 14; ++ i)
    {
        auto card = std::make_shared<CardObject>(i + 100, i + 1);
        card->setPosition(glm::vec3{3.0f, 0.005f * i, 3.0f});
        card->setRotation(glm::vec3{90.0f, 0.0f, 0.0f});
        card->setGroupId(Pipeline::CARD);
        _pipeline->attach(card.get());
        _stack.push_back(card);
        card->stage();
    }

    /*static constexpr size_t numSpheres = 15;

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
    goldSphere->stage();*/

    //_cubemapHDR = lithium::CubemapHDR::load("assets/belfast_sunset_puresky_16k.hdr");
    //_cubemapHDR = lithium::CubemapHDR::load("assets/abandoned_tiled_room_16k.hdr");
    _cubemapHDR = lithium::CubemapHDR::load("assets/chess.hdr");
    //_cubemapHDR = lithium::CubemapHDR::load("assets/school_quad_8k.hdr");
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

    auto tile = std::make_shared<lithium::InstancedObject<glm::mat4>>(AssetFactory::getMeshes()->tile, std::vector<lithium::Object::TexturePointer>{});
    tile->setGroupId(Pipeline::TILES);
    _objects.push_back(tile);
    _pipeline->attach(tile.get());
    tile->stage();
    tile->setShaderCallback([this](lithium::Renderable* r, lithium::ShaderProgram* sp){
        sp->setUniform("u_hovered_id", _hoveredTileId);
    });

    input()->setCursorListener([this](float x, float y) {
        static int lastId{-1};
        x = x / windowWidth() * defaultFrameBufferResolution().x;
        y = y / windowHeight() * defaultFrameBufferResolution().y;
        int id = objectIdAt(static_cast<int>(x), static_cast<int>(y));
        if(id != lastId)
        {
            if(id > 99)
            {
                _hand.hover(id);
                _hoveredTileId = 0;
            }
            else
            {
                _hand.hover(0);
                _hoveredTileId = id;
            }
            lastId = id;
        }
        return true;
    });

    input()->setDragCallback([this](int button, int mods, const glm::vec2& start, const glm::vec2& current, const glm::vec2& delta, lithium::Input::DragState dragState) {
        static int draggingId{-1};
        switch(dragState)
        {
            case lithium::Input::DragState::START:
                if(_hand.hovered())
                {
                    draggingId = _hand.hovered()->id();
                }
                else
                {
                    draggingId = -1;
                }
                break;
            case lithium::Input::DragState::DRAG:
                if(draggingId == -1)
                {
                    break;
                }
                if(current.y - start.y > 32.0f)
                {
                    std::cout << "Discard: " << draggingId << std::endl;
                    draggingId = -1;
                }
                else if(current.y - start.y < -32.0f)
                {
                    std::cout << "Play: " << draggingId << std::endl;
                    _hand.cardById(draggingId)->play();
                    draggingId = -1;
                }
                break;
        }
        return true;
    });

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

    float triSide = 1.0f;
    float triHeight = sinf(glm::pi<float>() / 3.0f);

    std::vector<GLfloat> triVertices;/* = {
        -triSide * 0.5f, 0.0f, -triHeight * 0.5f,      0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        0.0f, 0.0f, triHeight * 0.5f,      0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        triSide * 0.5f, 0.0f, -triHeight * 0.5f,      0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        triSide, 0.0f, triHeight * 0.5f,      0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
    };
    std::vector<GLuint> triIndices = {
        0, 1, 2
    };*/

    const int numRows = 8;
    const int numCols = 21;
    const float rowh = numRows / 2;
    const float colh = numCols / 2;

    int tileId{0};

    for(float z{-rowh}; z < rowh; ++z)
    {
        int zMod = static_cast<int>(z + rowh) % 2;
        float ch = colh - glm::abs((z + 1) - ((z+1) > 0 ? 1 : 0)) * 3;
        for(float x{-ch}; x <= ch; ++x)
        {
            int xMod = static_cast<int>(x + colh) % 2;
            /*triVertices.insert(triVertices.end(), {
                triSide * x * 0.5f, 0.0f, triHeight * z + ((x % 2 == zMod) ? -triHeight : triHeight) * 0.5f,
                0.0f, 1.0f, 0.0f, // normal
                rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f
            });*/
            glm::vec3 color{rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f};
            color = glm::vec3{static_cast<float>(tileId) / 256.0f};
            //color = glm::vec3{0.5f, 0.0f, 0.0f};
            ++tileId;
            glm::vec3 c = glm::vec3{triSide * 0.5f * x, 0.0f, triHeight * z};
            std::cout << ' ' << c.x;
            /*if(x % 2 == zMod)
            {
                triVertices.insert(triVertices.end(), {
                    c.x, c.y, c.z - triHeight * 0.4f,   0.0f, 1.0f, 0.0f,   color.r, color.g, color.b,
                    c.x - triSide * 0.4f, c.y, c.z + triHeight * 0.4f,  0.0f, 1.0f, 0.0f,   color.r, color.g, color.b,
                    c.x + triSide * 0.4f, c.y, c.z + triHeight * 0.4f,  0.0f, 1.0f, 0.0f,   color.r, color.g, color.b
                });
            }
            else
            {
                triVertices.insert(triVertices.end(), {
                    c.x - triSide * 0.4f, c.y, c.z - triHeight * 0.4f,  0.0f, 1.0f, 0.0f,   color.r, color.g, color.b,
                    c.x, c.y, c.z + triHeight * 0.4f,   0.0f, 1.0f, 0.0f,   color.r, color.g, color.b,
                    c.x + triSide * 0.4f, c.y, c.z - triHeight * 0.4f,  0.0f, 1.0f, 0.0f,   color.r, color.g, color.b
                });
            }*/
            glm::mat4 model = glm::translate(glm::mat4{1.0f}, c);
            if(xMod == zMod)
            {
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3{0.0f, 1.0f, 0.0f});
            }
            //model = glm::scale(model, glm::vec3{0.94f});
            tile->addInstance(model);
        }
        std::cout << std::endl;
    }

    tile->allocateBufferData();
    tile->linkBuffer({
        lithium::AttributePointer<GL_FLOAT>{0, 4, sizeof(glm::mat4), (void*)0},
        lithium::AttributePointer<GL_FLOAT>{1, 4, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))},
        lithium::AttributePointer<GL_FLOAT>{2, 4, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))},
        lithium::AttributePointer<GL_FLOAT>{3, 4, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))},
    });

    /*auto triMesh = std::make_shared<lithium::Mesh>(std::vector<lithium::VertexArrayBuffer::AttributeType>{
        lithium::VertexArrayBuffer::AttributeType::VEC3,
        lithium::VertexArrayBuffer::AttributeType::VEC3,
        lithium::VertexArrayBuffer::AttributeType::VEC3
    }, triVertices);

    //triMesh->setDrawMode(GL_TRIANGLE_STRIP);

    auto triObject = std::make_shared<lithium::Object>(triMesh, std::vector<lithium::Object::TexturePointer>{});
    triObject->setPosition(glm::vec3{-numCols * triSide * 0.5f * 0.5f, 0.0f, -numRows * triHeight * 0.5f});
    triObject->setGroupId(Pipeline::TILES);
    _objects.push_back(triObject);
    _pipeline->attach(triObject.get());
    triObject->stage();*/

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

    input()->addPressedCallback(GLFW_MOUSE_BUTTON_LEFT, [this](int key, int mods) {
        auto mp = input()->mousePosition();
        std::cout << "mousePos: " << mp.x << ", " << mp.y << std::endl;
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_KP_SUBTRACT, [this](int key, int mods) {
        _lod -= 0.25f;
        _lod = std::max(0.0f, _lod);
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_Q, [this](int key, int mods) {
        auto it = _stack.begin();// + (rand() % _stack.size());
        if(it == _stack.end())
        {
            return false;
        }
        (*it)->draw();
        _hand.addCard(*it);
        _stack.erase(it);
        return true;
    });

    input()->addPressedCallback(GLFW_KEY_TAB, [this](int key, int mods) {
        _hand.cycleHover();
        return true;
    });

    setMaxFps(120.0f);

    // Set the camera oirigin position and target.
    _pipeline->camera()->setTarget(glm::vec3{0.0f, 1.0f, 0.0f});
    _pipeline->camera()->setTarget(_cameraTarget);

    printf("%s\n", glGetString(GL_VERSION));
}

App::~App() noexcept
{
    _pipeline = nullptr;
    _background = nullptr;
    _objects.clear();
}

// Screen coordinates (cursor position) not normalized device coordinates.
int App::objectIdAt(int x, int y)
{
    auto fbo = _pipeline->offscreenBuffer();
    fbo->bind();
    static GLubyte pixel[3];
    // if necessary we can divide ids in r g b and get one byte per object type for eg.
    fbo->readPixel(x, fbo->resolution().y - y, GL_RED, GL_UNSIGNED_BYTE, pixel);
    fbo->unbind();
    return static_cast<int>(pixel[0]);
}

void App::update(float dt)
{
    lithium::Updateable::update(dt);
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
    float camX = sin(_cameraAngle) * _cameraRadius;
    float camZ = cos(_cameraAngle) * _cameraRadius;
    _pipeline->camera()->setPosition(_pipeline->camera()->target() + glm::vec3{camX, _camY, camZ});
    _hand.update(dt);
    _cubemapHDR->bind(GL_TEXTURE0);
    _pipeline->setTime(time());
    _pipeline->render();
    //lithium::CubemapHDR::draw(*_cubemapHDR, *_pipeline->camera());
}

void App::onWindowSizeChanged(int width, int height)
{
    _pipeline->setResolution(glm::ivec2{width, height});
}