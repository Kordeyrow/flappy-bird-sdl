#pragma once
#include <state_machine_base/state_machine.h>

class GameContext;

class GameState : public State {
public:
    GameState(GameContext* context, StateMachineEventEmitter* emitter)
        : State{ emitter }, context{ context } {}
protected:
    GameContext* context;
};

