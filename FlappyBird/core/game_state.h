#pragma once
#include <state_machine_base/state_machine.h>

class Game;

class GameState : public State {
public:
    GameState(Game* game, StateMachineEventEmitter* emitter)
        : State{ emitter }, game{ game } {}
protected:
    Game* game;
};

