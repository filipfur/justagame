#include "assetfactory.h"

#include "glplane.h"

AssetFactory::AssetFactory()
{

}

AssetFactory::~AssetFactory() noexcept
{

}

using attr = lithium::VertexArrayBuffer::AttributeType;
static constexpr attr POSITION{attr::VEC3};
static constexpr attr NORMAL{attr::VEC3};
static constexpr attr UV{attr::VEC2};
static constexpr attr BONE_IDS{attr::VEC4};
static constexpr attr BONE_WEIGHTS{attr::VEC4};
static constexpr attr COLOR{attr::VEC3};

const std::vector<attr> AssetFactory::objectAttributes{POSITION, NORMAL, UV};
const std::vector<attr> AssetFactory::modelAttributes{POSITION, NORMAL, UV, BONE_IDS, BONE_WEIGHTS};

void AssetFactory::loadMeshes()
{
    AssetFactory& instance = getInstance();
    instance._meshes.screen.reset(lithium::Plane2D());
    instance._meshes.card = lithium::tinyobjloader_load("assets/card/card.obj", objectAttributes);
    instance._meshes.sphere = lithium::tinyobjloader_load("assets/sphere.obj", objectAttributes);
}

void AssetFactory::loadTextures()
{
    AssetFactory& instance = getInstance();
    instance._textures.logoDiffuse.reset((lithium::ImageTexture*)lithium::ImageTexture::load("assets/Kraxbox_logo_lithium_metal_2ff2069c-b84a-426c-bf92-e9831105a5df.png", GL_SRGB_ALPHA, GL_RGBA)->setFilter(GL_NEAREST));
    instance._textures.card.reset((lithium::ImageTexture*)lithium::ImageTexture::load("assets/card/test.png", GL_SRGB_ALPHA, GL_RGBA)->setFilter(GL_LINEAR));
    instance._textures.skybox = lithium::Cubemap::load({
        "assets/skybox_/right.jpg",
        "assets/skybox_/left.jpg",
        "assets/skybox_/top.jpg",
        "assets/skybox_/bottom.jpg",
        "assets/skybox_/front.jpg",
        "assets/skybox_/back.jpg"
    });
    instance._textures.skybox->setFilter(GL_LINEAR);

    instance._textures.rustedIron.push_back(std::shared_ptr<lithium::ImageTexture>(
        lithium::ImageTexture::load("assets/rustediron1-alt2-bl/rustediron2_basecolor.png", GL_SRGB, GL_RGB)));
    instance._textures.rustedIron.push_back(std::shared_ptr<lithium::ImageTexture>(
        lithium::ImageTexture::load("assets/rustediron1-alt2-bl/rustediron2_normal.png", GL_RGB, GL_RGB)));
    instance._textures.rustedIron.push_back(std::shared_ptr<lithium::ImageTexture>(
        lithium::ImageTexture::load("assets/rustediron1-alt2-bl/rustediron2_metallic.png", GL_RED, GL_RED)));
    instance._textures.rustedIron.push_back(std::shared_ptr<lithium::ImageTexture>(
        lithium::ImageTexture::load("assets/rustediron1-alt2-bl/rustediron2_roughness.png", GL_RED, GL_RED)));
    instance._textures.rustedIron.push_back(std::shared_ptr<lithium::ImageTexture>(
        lithium::ImageTexture::load("assets/rustediron1-alt2-bl/rustediron2_ao.png", GL_RGB, GL_RGB)));
}

void AssetFactory::loadObjects()
{
    AssetFactory& instance = getInstance();
    instance._gltfLoader.loadObjects(instance._objects.windTurbine, "assets/windturbine/windturbine.gltf");
}

void AssetFactory::loadFonts()
{
    AssetFactory& instance = getInstance();
}

const AssetFactory::Meshes* AssetFactory::getMeshes()
{
    return &AssetFactory::getInstance()._meshes;
}

const AssetFactory::Textures* AssetFactory::getTextures()
{
    return &AssetFactory::getInstance()._textures;
}

const AssetFactory::Objects* AssetFactory::getObjects()
{
    return &AssetFactory::getInstance()._objects;
}

const AssetFactory::Fonts* AssetFactory::getFonts()
{
    return &AssetFactory::getInstance()._fonts;
}

AssetFactory& AssetFactory::getInstance()
{
    static AssetFactory instance;
    return instance;
}
