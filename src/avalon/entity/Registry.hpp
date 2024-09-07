#pragma once

#include "Actor.hpp"

// The `Registry` class manages the creation, destruction, and component management for entities in a game or simulation.
// It allows entities to be composed of various components, and provides mechanisms for efficiently iterating over entities
// that have specific components. This class is central to the Entity-Component-System (ECS) architecture, enabling decoupled management of entity behavior and data.
//
// Example Usage:
//
// void update(Registry &registry) {
//     // Retrieve a view of all entities that have both Position and Velocity components.
//     auto view = registry.view<const Position, Velocity>();
//
//     // Example 1: Use a callback to process each entity's components
//     view.each([](const auto &pos, auto &vel) {
//         // Example logic: Update velocity based on position
//         vel.dx += pos.x;
//         vel.dy += pos.y;
//     });
//
//     // Example 2: Use a range-based for loop to process entities and their components
//     for (auto [entity, pos, vel] : view.each()) {
//         // Example logic: Perform operations on each entity
//     }
//
//     // Example 3: Use forward iterators to access and modify individual components
//     for (auto entity : view) {
//         auto &vel = view.get<Velocity>(entity);
//         // Example logic: Modify velocity directly
//         vel.dx += 1;
//     }
// }
// ```
class Registry {
public:
    Actor createActor();

    void destroyActor(const Actor &entity);

    template<typename... Components>
    class View {
    public:
        View(Registry &registry) : registry(registry) {}

        // Callback-based iteration
        template<typename Func>
        void each(Func func) const;

        // Range-based for loop
        auto each() const;

        // Forward iterators
        class Iterator {
        public:
            Iterator(typename std::vector<std::tuple<Actor, Components...>>::const_iterator it) : it(it) {}

            bool operator!=(const Iterator &other) const { return it != other.it; }

            Iterator &operator++() {
                ++it;
                return *this;
            }

            std::tuple<Actor, Components...> operator*() const { return *it; }

        private:
            typename std::vector<std::tuple<Actor, Components...>>::const_iterator it;
        };

        Iterator begin() const {
            return Iterator(data.begin());
        }

        Iterator end() const {
            return Iterator(data.end());
        }

        template<typename T>
        T &get(Actor entity) const {
            auto component = entity.getComponent<T>();
            if (component) {
                return *component;
            }
            return nullptr;
        }

    private:
        // Base case: No more components to check
        template<typename Actor, typename Component>
        bool hasComponent(const Actor &actor) {
            return actor.getComponent<Component>() != nullptr;
        }

        /*
         * Checks recursively if the actor has the components specified
         */
        template<typename Actor, typename Component, typename... Components>
        bool hasAllComponents(const Actor &actor) {
            return hasComponent<Actor, Component>(actor) && hasAllComponents<Actor, Components...>(actor);
        }

        // End of recursion
        template<typename Actor>
        bool hasAllComponents(const Actor &actor) {
            return true;
        }

        Registry &registry;
        mutable std::vector<std::tuple<Actor, Components...>> data;  // To store entities and their components
    };

    template<typename... Components>
    View<Components...> view() {
        return View<Components...>(*this);
    }

    friend void to_json(json &j, const Registry &registry);

    friend void from_json(const json *j, Registry &registry);

private:
    std::unordered_map<ActorId, Actor> entities;
    ActorId nextEntityID = 0;
};
