#pragma once
#include "SDL.h"
#include <iostream>
#include <math/vector2.h>
#include <system_component/transform.h>
#include <system_component/drawable.h>
#include <system_component/updatable.h>
#include <system_component/entity.h>
#include <system_component/collider.h>

class FlappyBird : public Transform, public Drawable, public Updatable, public Entity, public CircleCollider {
public:
    FlappyBird(SDL_Texture* texture, double ground_y, Vector2 position, Vector2 size, double circle_collider_radius, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : Transform{ position, size },
        Drawable{ texture, this, -10, flip },
        Entity{ std::set<TAG>{ PLAYER } },
        CircleCollider{ this, this, circle_collider_radius },
        ground_y{ ground_y } {}

    CircleCollider* circle_collider() { return this; }

    void update(double elapsed_time) override {
        if (_fallen) {
            return;
        }

        // gravity
        speed_y += relative_gravity * elapsed_time;
        position = Vector2{ position.x, position.y + speed_y * elapsed_time * inverted_y_axis };

        // ground check
        double max_y = ground_y - calc_scaled_size().y / 2;
        if (position.y >= max_y) {
            _dead = true;
            _fallen = true;
            position.y = max_y;
        }

        if (position.y < 0) {
            speed_y = 0;
            position.y = 0;
        }
    }

    void collided(Collider* other) override {

    }

    void jump() {
        if (_dead) {
            return;
        }

        //std::cout << "jump" << std::endl;
        speed_y = jump_force * world_space_proportion;
    }

    void die() {
        if (_dead) {
            return;
        }
        _dead = true;

        //std::cout << "died" << std::endl;
    }

    void kill() {
        _dead = true;
    }

    bool dead() const {
        return _dead;
    }

    const bool& is_dead_ref() const { return _dead; }

    bool fallen() const {
        return _fallen;
    }

private:
    double ground_y;
    bool _fallen = false;
    bool _dead = false;
    float speed_y = 0;
    const int inverted_y_axis = -1;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 130;
    const float extra_gravity = -3;
    const float relative_gravity = gravity * world_space_proportion + extra_gravity * world_space_proportion;
};

