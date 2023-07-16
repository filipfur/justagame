#pragma once

#include <memory>
#include "glimagetexture.h"

struct Card
{
    std::shared_ptr<lithium::ImageTexture> texture;
    std::string title{"Untitled"};
    std::string body{""};
    int timeCost{0};
    int cashCost{0};
    int energyCost{0};
    int reputationCost{0};
    int educationLevel{0};
};