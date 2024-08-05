#pragma once

#include <memory>
#include <string>
//#include <filesystem>
#include <sstream>
#include <vector>
#include <queue>
//#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
//#include <cstdint>
#include <utility>
#include <typeindex>

#include "Log.hpp"
#include "../event/InputListeners.hpp"

#define BIT(x) (1 << x)


template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args &&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using WeakRef = std::weak_ptr<T>;

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