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

#include <nlohmann/json.hpp>
using json = nlohmann::json;


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

template<typename T>
class Switch {
public:
    using CaseFunction = std::function<void()>;

    // Add a case
    Switch& Case(const T& value, CaseFunction func) {
        cases[value] = func;
        return *this;
    }

    // Add a default case
    Switch& Default(CaseFunction func) {
        defaultCase = func;
        return *this;
    }

    // Execute the switch
    void Execute(const T& value) const {
        auto it = cases.find(value);
        if (it != cases.end()) {
            it->second();
        } else if (defaultCase) {
            defaultCase();
        }
    }

private:
    std::unordered_map<T, CaseFunction> cases;
    CaseFunction defaultCase = nullptr;
};

template<typename T>
Switch<T> CreateSwitch() {
    return Switch<T>();
}


