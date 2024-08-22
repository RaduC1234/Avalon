#pragma once

#include "avalon/core/Core.hpp"

class Registry {
public:
    using Entity = std::size_t;
    using ComponentID = std::type_index;

    Registry() : next_entity(0) {}

    Entity create() {
        entities.push_back(next_entity);
        return next_entity++;
    }

    // Remove an entity
    void destroy(Entity entity) {
        // For simplicity, just mark the entity as invalid
        // A more robust system would need to handle this properly
        // by removing its components and reusing the entity ID.
        // This example will just ignore the entity.
    }

    // Add a component to an entity
    template<typename T>
    void emplace(Entity entity, T component) {
        auto id = typeid(T);
        components[id][entity] = std::make_shared<T>(component);
    }

    // Get a component from an entity
    template<typename T>
    T& get(Entity entity) {
        auto id = typeid(T);
        return *std::static_pointer_cast<T>(components[id][entity]);
    }

    // View entities with specific components
    template<typename... Ts>
    class View {
    public:
        View(Registry& registry) : registry(registry) {}

        class Iterator {
        public:
            Iterator(Registry& registry, std::vector<Entity>& entities)
                    : registry(registry), entities(entities), index(0) {
                // Move to the first valid entity
                advance_to_valid();
            }

            bool operator!=(const Iterator& other) const {
                return index != other.index;
            }

            Iterator& operator++() {
                ++index;
                advance_to_valid();
                return *this;
            }

            auto operator*() {
                return get_components();
            }

        private:
            void advance_to_valid() {
                while (index < entities.size() && !has_all_components(entities[index])) {
                    ++index;
                }
            }

            bool has_all_components(Entity entity) const {
                return ((registry.components.count(typeid(Ts)) && registry.components[typeid(Ts)].count(entity)) && ...);
            }

            std::tuple<Ts&...> get_components() {
                return std::tuple<Ts&...>(
                        registry.get<Ts>(entities[index])...
                );
            }

            Registry& registry;
            std::vector<Entity>& entities;
            std::size_t index;
        };

        Iterator begin() {
            return Iterator(registry, registry.entities);
        }

        Iterator end() {
            return Iterator(registry, registry.entities);
        }

    private:
        Registry& registry;
    };

    template<typename... Ts>
    View<Ts...> view() {
        return View<Ts...>(*this);
    }

private:
    std::vector<Entity> entities;
    std::unordered_map<ComponentID, std::unordered_map<Entity, std::shared_ptr<void>>> components;
    Entity next_entity;
};
