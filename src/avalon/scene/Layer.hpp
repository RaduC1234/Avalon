#pragma once

#include "avalon/core/Core.hpp"

class Layer {
public:
    explicit Layer(std::string layerName = "layer")
            : layerName(std::move(layerName)) {}

    virtual ~Layer() = default;

    virtual void onAttach() {}

    virtual void onDetach() {}

    virtual void onUpdate(float deltaTime) {}

    virtual void onImGuiRender() {}

    const std::string& getLayerName() const { return layerName; }

protected:
    bool defaultState = false;
    std::string layerName;
};

class GameLayer : protected Layer {
public:
    GameLayer(int zIndex) : Layer("Game Layer " + std::to_string(zIndex)), zIndex(zIndex){}



    std::vector<EntityID> entities;
    int zIndex;

    float parallaxOffsetSpeedX = 0.0f;
    float parallaxOffsetSpeedY = 0.0;
};

/**
 * Abstracts the layer system.
 */
class LayerStack {
public:
    LayerStack() = default;

    ~LayerStack() {
        for (auto& layer : layers) {
            layer->onDetach();
        }
    }

    void pushLayer(Scope<Layer> layer) {
        layer->onAttach();
        layers.emplace(layers.begin() + layerInsertIndex, std::move(layer));
        layerInsertIndex++;
    }

    void pushOverlay(Scope<Layer> overlay) {
        overlay->onAttach();
        layers.emplace_back(std::move(overlay));
    }

    void popLayer(Layer* layer) {
        auto it = std::find_if(layers.begin(), layers.begin() + layerInsertIndex,
                               [layer](const Scope<Layer>& l) { return l.get() == layer; });
        if (it != layers.begin() + layerInsertIndex) {
            (*it)->onDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void popOverlay(Layer* overlay) {
        auto it = std::find_if(layers.begin() + layerInsertIndex, layers.end(),
                               [overlay](const Scope<Layer>& l) { return l.get() == overlay; });
        if (it != layers.end()) {
            (*it)->onDetach();
            layers.erase(it);
        }
    }

    class Iterator {
    public:
        using vector_iterator = std::vector<std::unique_ptr<Layer>>::iterator;

        Iterator(vector_iterator it) : current(it) {}

        Layer* operator*() const { return current->get(); }

        Iterator& operator++() {
            ++current;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return current != other.current; }

    private:
        vector_iterator current;
    };

    Iterator begin() noexcept { return Iterator(layers.begin()); }
    Iterator end() noexcept { return Iterator(layers.end()); }

private:
    std::vector<Scope<Layer>> layers;
    unsigned int layerInsertIndex = 0;
};
