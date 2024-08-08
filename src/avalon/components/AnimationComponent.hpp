#pragma once

#include "avalon/logic/Component.hpp"
#include "avalon/renderer/Texture.hpp"

struct Frame {
    Ref<Texture> texture;

};

class AnimationComponent : public Component {
    std::string animationFile;
};
