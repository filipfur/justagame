#pragma once

#include <memory>
#include <filesystem>
#include "glimagetexture.h"

struct Card
{
    std::shared_ptr<lithium::ImageTexture> texture;
    std::string title{"Untitled"};
    std::string body{""};
    std::filesystem::path path;
    int timeCost{0};
    int cashCost{0};
    int energyCost{0};
    int reputationCost{0};
    int educationLevel{0};
};