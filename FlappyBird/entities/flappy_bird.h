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
    FlappyBird(SDL_Texture* texture, Vector2 position, Vector2 size) 
        : Transform{ position, size },
        Drawable{ texture, this, -10},
        Entity{ std::set<TAG>{ PLAYER } },
        Collider{ this, this, Vector2{0.8, 0.8} } {
    
    }

    void update(double elapsed_time) override {
        // gravity
        speed_y += relative_gravity * elapsed_time;
        position = Vector2{ position.x, position.y + speed_y * elapsed_time * inverted_y_axis };
    }

    void collided(Collider* other) override {

    }

    void jump() {
        //std::cout << "jump" << std::endl;
        speed_y = jump_force * world_space_proportion;
    }

    void die() {
        std::cout << "die" << std::endl;
    }

private:
    float speed_y = 0;
    const int inverted_y_axis = -1;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};

