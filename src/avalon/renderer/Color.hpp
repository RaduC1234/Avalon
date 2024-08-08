#pragma once

#include <glm/glm.hpp>

class Color {
public:
    float r, g, b, a;

    Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

    Color(int r, int g, int b, int a = 255) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

    Color(const std::string &hex) {
        if (hex.length() == 8) {
            r = std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f;
            g = std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f;
            b = std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f;
            a = std::stoi(hex.substr(6, 2), nullptr, 16) / 255.0f;
        } else {
            AV_CORE_ERROR("Invalid hex color provided");
        }
    }

    // Cast to glm::vec4
    explicit operator glm::vec4() const {
        return {r, g, b, a};
    }
};
