#include "pipeline.h"
#include "glplane.h"

Pipeline::Pipeline(const glm::ivec2& resolution) : lithium::RenderPipeline{resolution},
    _camera{new lithium::SimpleCamera(glm::perspective(glm::radians(45.0f), (float)resolution.x / (float)resolution.y, 0.1f, 100.0f))},
    _frameBuffer{std::make_shared<lithium::FrameBuffer>(resolution)},
    _cameraUBO{std::make_shared<lithium::UniformBufferObject>(sizeof(glm::mat4) * 2 + sizeof(glm::vec3), "CameraUBO", 0)}
{
    enableDepthTesting();
    depthFunc(GL_LESS);
    enableBlending();
    blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enableMultisampling();
    enableFaceCulling();

    _blockShader = std::make_shared<lithium::ShaderProgram>("shaders/object.vert", "shaders/object.frag");
    _blockShader->setUniform("u_texture_0", 0);
    _blockShader->setUniform("u_projection", _camera->projection());

    _pbrShader = std::make_shared<lithium::ShaderProgram>("shaders/object.vert", "shaders/pbr.frag");
    _pbrShader->setUniform("u_texture_0", 0);
    _pbrShader->setUniform("u_projection", _camera->projection());

    _skyboxShader = std::make_shared<lithium::ShaderProgram>("shaders/cubemap.vert", "shaders/cubemap.frag");
    //_skyboxShader->setUniform("u_projection", _camera->projection());

    _screenShader = std::make_shared<lithium::ShaderProgram>("shaders/screenshader.vert", "shaders/screenshader.frag");
    
    _msaaShader = std::make_shared<lithium::ShaderProgram>("shaders/screenshader.vert", "shaders/msaa.frag");
    _msaaShader->setUniform("u_texture", 0);
    _msaaShader->setUniform("u_resolution", resolution);

    _frameBuffer->bind();
    _frameBuffer->createTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D_MULTISAMPLE);
    _frameBuffer->createTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8, GL_FLOAT, GL_TEXTURE_2D_MULTISAMPLE);
    _frameBuffer->declareBuffers();
    _frameBuffer->unbind();

    _cameraUBO->bindBufferBase({
        _skyboxShader.get()});

    _cameraUBO->bufferSubData(0, _camera->projection());

    _screenMesh = std::shared_ptr<lithium::Mesh>(lithium::Plane2D());

    _screenGroup = createRenderGroup([this](lithium::Renderable* renderable) -> bool {
        return renderable->groupId() == BACKGROUND;
    });

    _skyboxGroup = createRenderGroup([this](lithium::Renderable* renderable) -> bool {
        return renderable->groupId() == SKYBOX;
    });

    _pbrGroup = createRenderGroup([this](lithium::Renderable* renderable) -> bool {
        if(auto object = dynamic_cast<lithium::Object*>(renderable))
        {
            return object->mesh()->material() != nullptr;
        }
        return false;
    });

    _mainGroup = createRenderGroup([this](lithium::Renderable* renderable) -> bool {
        return dynamic_cast<lithium::Object*>(renderable) && !renderable->hasAttachments();
    });

    _mainStage = addRenderStage(std::make_shared<lithium::RenderStage>(_frameBuffer, [this](){
        _cameraUBO->bufferSubData(sizeof(glm::mat4), _camera->view());
        _cameraUBO->bufferSubData(sizeof(glm::mat4) * 2, _camera->position());
        clearColor(0.0f, 0.0f, 0.0f, 0.0f);
        clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //disableDepthWriting();
        //_screenGroup->render(_screenShader.get());
        //enableDepthWriting();

        //_skyboxShader->setUniform("u_view", glm::mat4(glm::mat3(_camera->view())));
        disableDepthWriting();
        _skyboxGroup->render(_skyboxShader);
        enableDepthWriting();

        _pbrShader->setUniform("u_view", _camera->view());
        _pbrGroup->render(_pbrShader.get());

        _blockShader->setUniform("u_view", _camera->view());
        _blockShader->setUniform("u_time", 0.0f);
        _mainGroup->render(_blockShader.get());
    }));

    _finalStage = addRenderStage(std::make_shared<lithium::RenderStage>(nullptr, [this](){
        setViewportToResolution();
        clearColor(0.0f, 0.0f, 0.0f, 1.0f);
        clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        _msaaShader->use();
        _mainStage->frameBuffer()->texture(GL_COLOR_ATTACHMENT0)->bind(GL_TEXTURE0);
        _mainStage->frameBuffer()->bindTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE1);
        _screenMesh->bind();
        _screenMesh->draw();
    }));

}

void Pipeline::setResolution(const glm::ivec2& resolution)
{
    lithium::RenderPipeline::setResolution(resolution);
    //_msaaShader->setUniform("u_resolution", resolution);
    //_camera->setProjection(glm::perspective(glm::radians(45.0f), (float)resolution.x / (float)resolution.y, 0.1f, 100.0f));
    //_blockShader->setUniform("u_projection", _camera->projection());
}

Pipeline::~Pipeline()
{
    _blockShader = nullptr;
    _screenShader = nullptr;
    _msaaShader = nullptr;
    _screenMesh = nullptr;
}