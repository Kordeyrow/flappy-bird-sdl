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
#include <core/game_base.h>

class Pipe : public Transform, public Drawable, public Updatable, public Entity, public Collider {
public:
    Pipe(GameBase* game, SDL_Texture* texture, Vector2 position, Vector2 size, float speed_x, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : game{ game},
        Transform{ position, size },
        Drawable{ texture, this, 0, flip },
        Entity{}, speed_x{ speed_x },
        Collider{ this, this, Vector2{0.95, 0.95} } {}

    void update(double elapsed_time) override {
        if (game->game_over()) {
            return;
        }

        // gravity
        position = Vector2{ position.x + speed_x * elapsed_time, position.y };
    }

    void collided(Collider* other) override {
        if (game->game_over() || collided_with_player) {
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
    GameBase* game;
    bool collided_with_player = false;
    float speed_x = -100;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};
