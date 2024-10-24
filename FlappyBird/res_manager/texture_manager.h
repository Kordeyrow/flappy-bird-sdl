#pragma once
#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <SDL.h>

class TextureManager {
public:
	enum TEXTURE_PATH_KEY {
		TEXTURE_FLAPPY_BIRD_UP_WING
	};
	TextureManager(SDL_Renderer* _renderer)
	{
		renderer = _renderer;
	}
	bool load_init_textures()
	{
		for (TEXTURE_PATH_KEY key : textures_to_load_on_init)
		{
			if (load_texture(key) == nullptr) {
				return false;
			}
		}
		return true;
	}
	SDL_Texture* get_texture(TEXTURE_PATH_KEY key)
	{
		auto iterator = loaded_textures.find(key);
		if (iterator != loaded_textures.end())
			return iterator->second;
		return load_texture(key);
	}
	void kill()
	{
		for (auto& t : loaded_textures) {
			SDL_DestroyTexture(t.second);
		}
		loaded_textures.clear();
	}

private:
	// config
	std::map<TEXTURE_PATH_KEY, const char*> textures_paths{
		{ static_cast<TEXTURE_PATH_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "../res/sprites/yellowbird-upflap.bmp" }
	};
	std::array<TEXTURE_PATH_KEY, 1> textures_to_load_on_init{
		static_cast<TEXTURE_PATH_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING)
	};
	// shared
	SDL_Renderer* renderer;
	// runtime
	std::map<TEXTURE_PATH_KEY, SDL_Texture*> loaded_textures;

	SDL_Texture* load_texture(TEXTURE_PATH_KEY key)
	{
		// Load bitmap into surface
		SDL_Surface* buffer = SDL_LoadBMP(textures_paths[key]);
		if (!buffer) {
			std::cout << "Error loading sprite: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		// Create texture
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, buffer);
		// Free surface as it's no longer needed
		SDL_FreeSurface(buffer);
		buffer = nullptr;
		if (!texture) {
			std::cout << "Error creating sprite: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		loaded_textures.emplace(key, texture);

		return texture;
	}
};

