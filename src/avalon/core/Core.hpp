#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <set>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <functional>
#include <chrono>
#include <utility>
#include <typeindex>

#include "Log.hpp"

#include "avalon/event/InputListeners.hpp"

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



