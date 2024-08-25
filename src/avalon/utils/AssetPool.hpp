#pragma once

#include "ResourceBundle.hpp"

class AssetPool {
public:

    static auto loadBundle(const std::string &name) {
        bundles[name] = new ResourceBundle(name);
        return bundles[name];
    }

    static auto getBundle(const std::string& name) {
        return bundles[name];
    }

    static void unloadBundle(const std::string& name) {
        delete bundles[name];
    }

    static void unloadAll() {
        for(auto& x : bundles) {
            delete x.second;
        }
    }

private:
    static inline std::unordered_map<std::string, ResourceBundle *> bundles;
};


