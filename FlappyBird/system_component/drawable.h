#pragma once
#include <SDL.h>
#include <math/vector2.h>
#include <system_component/transform.h>

class Drawable {

public:
	Drawable(SDL_Texture* texture, Transform* t, int layer_index = 0, SDL_RendererFlip flip = SDL_FLIP_NONE)
		: texture{ texture }, draw_transform{ t }, _layer_index{ layer_index }, _flip { flip } {}

	SDL_Texture* get_texture() const {
		return texture;
	}

	SDL_Rect get_rect(){
		return SDL_Rect{ (int)(draw_transform->position.x -draw_transform->size.x/2), (int)(draw_transform->position.y -draw_transform->size.y/2), (int)draw_transform->size.x, (int)draw_transform->size.y };
	}

	double get_rotation() const {
		return draw_transform->rotation;
	}

	int layer_index() const {
		return _layer_index;
	}

	SDL_RendererFlip flip() const {
		return _flip;
	}

protected:
	SDL_RendererFlip _flip;
	int _layer_index;
	SDL_Texture* texture;
	Transform* draw_transform;
};

