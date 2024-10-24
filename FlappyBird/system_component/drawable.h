#pragma once
#include <SDL.h>
#include "../math/vector2.h"

class Drawable {

public:
	Drawable(SDL_Texture* texture, Vector2 position, Vector2 size)
		: texture{ texture },
		position{ position },
		rect{ (int)position.x, (int)position.y, (int)size.x, (int)size.y },
		rotation{ 0 } {}

	SDL_Texture* get_texture() const {
		return texture;
	}

	const SDL_Rect& get_rect() {
		rect = SDL_Rect{ (int)position.x, (int)position.y, (int)rect.w, (int)rect.h };
		return rect;
	}

	double get_rotation() const {
		return rotation;
	}

protected:
	SDL_Texture* texture;
	SDL_Rect rect;
	Vector2 position;
	double rotation;
};

