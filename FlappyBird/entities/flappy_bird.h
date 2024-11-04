#pragma once
#include "SDL.h"
#include <iostream>
#include <math/vector2.h>
#include <system_component/transform.h>
#include <system_component/drawable.h>
#include <system_component/updatable.h>
#include <system_component/entity.h>
#include <system_component/collider.h>

class FlappyBird : public Transform, public Drawable, public Updatable, public Entity, public Collider {
public:
    FlappyBird(SDL_Texture* texture, double ground_y, Vector2 position, Vector2 size, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : Transform{ position, size },
        Drawable{ texture, this, -10, flip },
        Entity{ std::set<TAG>{ PLAYER } },
        Collider{ this, this, Vector2{0.96, 0.96} },
        ground_y{ ground_y } {}

    void update(double elapsed_time) override {
        if (_fallen) {
            return;
        }

        // gravity
        speed_y += relative_gravity * elapsed_time;
        position = Vector2{ position.x, position.y + speed_y * elapsed_time * inverted_y_axis };

        // ground check
        double bottom = position.y + calc_scaled_size().y / 2;
        if (bottom >= ground_y) {
            _dead = true;
            _fallen = true;
            position.y = ground_y;
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
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};

