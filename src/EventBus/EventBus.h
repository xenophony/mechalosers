#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <entt/entt.hpp>

class EventBus {
public:
    entt::dispatcher dispatcher;

    template<typename Event, typename... Args>
    void Emit(Args&&... args) {
        dispatcher.trigger<Event>(std::forward<Args>(args)...);
    }

    template<typename Event, auto Candidate, typename Type>
    void Subscribe(Type& value_or_instance) {
        dispatcher.sink<Event>().template connect<Candidate>(value_or_instance);
    }

    void Clear() {
        dispatcher.clear();
    }
};


#endif


