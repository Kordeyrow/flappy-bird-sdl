#pragma once
#include "SDL.h"
#include <iostream>
#include "../math/vector2.h"
#include "../system_component/drawable.h"

class FlappyBird : public Drawable {
public:
    FlappyBird(SDL_Texture* texture, Vector2 position, Vector2 size) : Drawable{ texture, position, size } {}

    void update(double elapsed_time) {
        // gravity
        speed_y += relative_gravity * elapsed_time;
        position = Vector2{ position.x, position.y + speed_y * elapsed_time * inverted_y_axis };
    }

    void jump() {
        //std::cout << "jump" << std::endl;
        speed_y = jump_force * world_space_proportion;
    }

private:
    float speed_y = 0;
    const int inverted_y_axis = -1;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};

