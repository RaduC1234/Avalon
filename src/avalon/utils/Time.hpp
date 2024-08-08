#pragma once

#include "avalon/core/Core.hpp"

class Time {
public:
    inline static std::chrono::time_point<std::chrono::high_resolution_clock> timeStarted;

    inline static void init() {
        timeStarted = std::chrono::high_resolution_clock::now();
    }

    inline static float getTime() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - timeStarted;
        return elapsed.count();
    }
};
