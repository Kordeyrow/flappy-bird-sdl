#pragma once
#include <state_machine_base/state_machine.h>

class GameState : public State {
public:
    GameState(StateMachineEventEmitter* emitter) : State{ emitter } {}
};

