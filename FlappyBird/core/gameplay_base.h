#pragma once
#include "game_state.h"

class GameplayBase : public GameState {
public:
	GameplayBase(StateMachineEventEmitter* emitter) : GameState{ emitter } {}
};

