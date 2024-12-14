#pragma once
#include "SDL.h"
#include <iostream>
#include <math/vector2.h>
#include <system_component/transform.h>
#include <system_component/drawable.h>
#include <system_component/updatable.h>
#include <system_component/entity.h>
#include <system_component/collider.h>
#include <entities/flappy_bird.h>
#include <entities/flappy_bird.h>
#include <state_machine_base/state_machine.h>
#include <utils/class_type_check.h>
#include <game/gameplay_base.h>

class Pipe : public StateMachineListener, public Transform, public Drawable, public Updatable, public Entity, public RectangleCollider {
public:
    Pipe(StateMachineEventEmitter* game_sm_emitter, GLuint texture, Vector2 position, Vector2 size, float speed_x, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : game_sm_emitter{ game_sm_emitter },
        Transform{ position, size },
        Drawable{ texture, this, 0, flip },
        Entity{}, speed_x{ speed_x },
        RectangleCollider{ this, this, Vector2{0.92, 0.99} }
    {
        auto current_state = game_sm_emitter->subscribe(this);
        check_new_game_state(current_state);
    }

    RectangleCollider* rectangle_collider() { return this; }

    ~Pipe() {
        game_sm_emitter->unsubscribe(this);
    }

    void check_new_game_state(State* state) {
        if (ClassTypeCheck::is_class_type<GameplayBase>(state)) {
            stop = true;
        }
    }

    // Inherited via StateMachineListener
    void on_state_changed(State* old_state, State* new_state) override {
        check_new_game_state(new_state);
    }

    void update(double elapsed_time) override {
        if (stop) {
            return;
        }
        // move left
        position = Vector2{ position.x + speed_x * elapsed_time, position.y };
    }

    void collided(Collider* other) override {
        if (stop || collided_with_player) {
            return;
        }

        // check collision with player 
        bool other_is_player = other->owner()->tags().find(PLAYER) != other->owner()->tags().end();
        if (other_is_player) {
            FlappyBird* player = static_cast<FlappyBird*>(other->owner());
            player->die();
            collided_with_player = true;
        }
    }

private:
    bool stop = false;
    StateMachineEventEmitter* game_sm_emitter;
    bool collided_with_player = false;
    float speed_x = -100;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};
