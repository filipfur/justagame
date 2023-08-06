#pragma once

#include <memory>
#include "glsimplecamera.h"
#include "glrenderpipeline.h"
#include "glframebuffer.h"
#include "gluniformbufferobject.h"

class Pipeline : public lithium::RenderPipeline
{
public:
    enum Group
    {
        DEFAULT,
        BACKGROUND,
        SKYBOX,
        PBR,
        PBR_POLY_HAVEN,
        CARD
    };

    Pipeline(const glm::ivec2& resolution);

    ~Pipeline() noexcept;

    std::shared_ptr<lithium::SimpleCamera> camera()
    {
        return _camera;
    }

    virtual void setResolution(const glm::ivec2& resolution) override;

    void setTime(float time)
    {
        _time = time;
    }

private:
    /* Shaders */
    std::shared_ptr<lithium::ShaderProgram> _blockShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _msaaShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _screenShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _skyboxShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _pbrShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _pbrPolyHavenShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _pbrBaseColorShader{nullptr};
    std::shared_ptr<lithium::ShaderProgram> _cardShader{nullptr};
    std::shared_ptr<lithium::SimpleCamera> _camera{nullptr};

    /*Render groups*/
    std::shared_ptr<lithium::RenderGroup> _screenGroup;
    std::shared_ptr<lithium::RenderGroup> _skyboxGroup;
    std::shared_ptr<lithium::RenderGroup> _pbrGroup;
    std::shared_ptr<lithium::RenderGroup> _pbrPolyHavenGroup;
    std::shared_ptr<lithium::RenderGroup> _pbrBaseColorGroup;
    std::shared_ptr<lithium::RenderGroup> _cardGroup;
    std::shared_ptr<lithium::RenderGroup> _mainGroup;

    /*Render stages*/
    std::shared_ptr<lithium::RenderStage> _mainStage;
    std::shared_ptr<lithium::RenderStage> _finalStage;

    /* Framebuffers */
    std::shared_ptr<lithium::FrameBuffer> _frameBuffer;
    std::shared_ptr<lithium::UniformBufferObject> _cameraUBO;
    
    /* Meshes */
    std::shared_ptr<lithium::Mesh> _screenMesh;

    float _time{0.0f};
};