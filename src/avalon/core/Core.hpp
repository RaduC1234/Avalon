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
#include <variant>

#include "Log.hpp"

#include "avalon/event/InputListeners.hpp"

#include <imgui.h>

#include <glm/glm.hpp>

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

    Switch& Case(const T& value, CaseFunction func) {
        cases[value] = func;
        return *this;
    }

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

namespace nlohmann {

    // Serialization for glm::vec2
    template <>
    struct adl_serializer<glm::vec2> {
        static void to_json(json& j, const glm::vec2& v) {
            j = json{{"x", v.x}, {"y", v.y}};
        }

        static void from_json(const json& j, glm::vec2& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };

    // Serialization for glm::vec3
    template <>
    struct adl_serializer<glm::vec3> {
        static void to_json(json& j, const glm::vec3& v) {
            j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
        }

        static void from_json(const json& j, glm::vec3& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
        }
    };

    // Serialization for glm::vec4
    template <>
    struct adl_serializer<glm::vec4> {
        static void to_json(json& j, const glm::vec4& v) {
            j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w}};
        }

        static void from_json(const json& j, glm::vec4& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
            j.at("w").get_to(v.w);
        }
    };
}


