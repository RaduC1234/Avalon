#include "Registry.hpp"

Actor Registry::createActor() {
    ActorId id = nextEntityID++;
    Actor entity(id, this);
    entities.emplace(id, entity);
    return entity;
}

void Registry::destroyActor(const Actor& entity) {
    entities.erase(entity.getId());
}

template<typename... Components>
auto Registry::View<Components...>::each() const {
    std::vector<std::tuple<Actor, Components...>> result;
    for (const auto& [entityID, actor] : registry.entities) {
        if (hasAllComponents<Components...>(actor)) {
            result.emplace_back(actor, *(actor.getComponent<Components>())...);
        }
    }
    return result;
}

template<typename... Components>
template<typename Func>
void Registry::View<Components...>::each(Func func) const {
    for (const auto& [entityID, actor] : registry.entities) {
        if (hasAllComponents<Components...>(actor)) {
            func(*(actor.getComponent<Components>())...);
        }
    }
}
