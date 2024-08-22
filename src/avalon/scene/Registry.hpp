#pragma once

#include "avalon/core/Core.hpp"
#include "Entity.hpp"

using ComponentID = std::type_index;

class Registry {
public:

    Entity createEntity();
    void destroyEntity(Entity entity);

    template<typename T> void addComponent(Entity entity, T component);
    template<typename T> void removeComponent(Entity entity);
    template<typename T> bool hasComponent(Entity entity) const;
    template<typename T> T& getComponent(Entity entity);


    // Add a component to an entity
    template<typename T>
    void emplace(Entity entity, T component);

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
};
