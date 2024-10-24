#pragma once
#include "SDL.h"
#include <iostream>
#include <math/vector2.h>
#include <system_component/drawable.h>
#include <system_component/updatable.h>

class Pipe : public Drawable, public Updatable {
public:
    Pipe(SDL_Texture* texture, Vector2 position, Vector2 size) : Drawable{ texture, position, size } {}

    void update(double elapsed_time) override {
        // gravity
        position = Vector2{ position.x + speed_x * elapsed_time, position.y };
    }

private:
    float speed_x = -100;
    const float jump_force = 4;
    const float gravity = -9.8;
    const float world_space_proportion = 90;
    const float relative_gravity = gravity * world_space_proportion;
};
