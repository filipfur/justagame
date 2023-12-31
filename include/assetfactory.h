#pragma once

#include "glfont.h"
#include "globject.h"
#include "glanimatedobject.h"
#include "gltinyobjloader.h"
#include "gltfloader.h"
#include "glskinnedobject.h"
#include "glcubemap.h"

class AssetFactory
{
public:
    struct Meshes
    {
        std::shared_ptr<lithium::Mesh> card;
        std::shared_ptr<lithium::Mesh> screen;
        std::shared_ptr<lithium::Mesh> sphere;
        std::shared_ptr<lithium::Mesh> tile;
    } _meshes;

    struct Textures
    {
        std::shared_ptr<lithium::ImageTexture> logoDiffuse;
        std::shared_ptr<lithium::ImageTexture> card;
        std::shared_ptr<lithium::Cubemap> skybox;
        std::vector<std::shared_ptr<lithium::Texture<unsigned char>>> rustedIron;
        std::vector<std::shared_ptr<lithium::Texture<unsigned char>>> scuffedGold;
        std::vector<std::shared_ptr<lithium::Texture<unsigned char>>> barberChair;
        std::vector<std::shared_ptr<lithium::Texture<unsigned char>>> chessSet;
    } _textures;

    struct Objects
    {
        std::vector<std::shared_ptr<lithium::Object>> windTurbine;
        std::vector<std::shared_ptr<lithium::Object>> barberChair;
        std::vector<std::shared_ptr<lithium::Object>> chessSet;
    } _objects;

    struct Fonts
    {
        std::shared_ptr<lithium::Font> righteousFont;
    } _fonts;

    AssetFactory();
    virtual ~AssetFactory() noexcept;
    AssetFactory(AssetFactory const&) = delete;
    void operator=(AssetFactory const&)  = delete;

    static void loadMeshes();
    static void loadTextures();
    static void loadObjects();
    static void loadFonts();

    static AssetFactory& getInstance();

    static const std::vector<lithium::VertexArrayBuffer::AttributeType> objectAttributes;
    static const std::vector<lithium::VertexArrayBuffer::AttributeType> modelAttributes;

    static const AssetFactory::Meshes* getMeshes();
    static const AssetFactory::Textures* getTextures();
    static const AssetFactory::Objects* getObjects();
    static const AssetFactory::Fonts* getFonts();

    gltf::Loader _gltfLoader;
};