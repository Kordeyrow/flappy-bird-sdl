#pragma once
#pragma once
#include <unordered_map>
#include <type_traits>
#include <utils/class_type_check.h>
#include "listener_id.h"
#include "state_machine_listener_base.h"

class StateMachineEventEmitter;
class State {
public:
    State(StateMachineEventEmitter* emitter) : emitter{ emitter } {}

    virtual void enter() {};
    virtual State* run() { return nullptr; };
    virtual void exit() {};
    virtual ~State() = default;

    template<typename T>
    bool is_type() {
        return ClassTypeCheck::is_class_type<T>(this);
    }
protected:
    StateMachineEventEmitter* emitter;
};

class StateMachineListener : public StateMachineListenerBase {
public:
    StateMachineListener() { _listener_id = next_id++; };
    virtual ~StateMachineListener() = default;

    virtual void on_state_changed(State* old_state, State* new_state) = 0;
    ListenerID listener_id() { return _listener_id; }
private:
    ListenerID _listener_id = 0;
};

class StateMachineEventEmitter {
public:
    StateMachineEventEmitter () {}

    // Listeners
    State* subscribe(StateMachineListener* listener) {
        listeners_map[listener->listener_id()] = listener;
        return current_state;
    }
    void unsubscribe(StateMachineListener* listener) {
        listeners_map.erase(listener->listener_id());
    }

protected:
    State* current_state;
    std::unordered_map<ListenerID, StateMachineListener*> listeners_map;
};

class StateMachine : public StateMachineEventEmitter {
public:
    StateMachine() {}
    
    void init(State* start_state) {
        current_state = start_state;
        current_state->enter();
    }

    State* run() {
        auto next_state = current_state->run();
        if (next_state != current_state) {
            change_state(next_state);
        }
        return current_state;
    }

    State* get_current_state() { return current_state; };

//private:
    void change_state(State* new_state) {
        // var swap
        State* old_state = current_state;
        current_state = new_state;
        // state changes
        old_state->exit();
        new_state->enter();
        // event
        for (auto& pair : listeners_map) {
            pair.second->on_state_changed(old_state, new_state);
        }
    }
};
